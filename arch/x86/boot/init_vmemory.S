#include <boot/multiboot2.h>
#include <arch/x86/cpuid.h>
#include <arch/x86/register.h>
#include <arch/x86/page_define.h>
#include <mem/memorymap.h>

.extern _kernel_image_phy_start_address
.extern _kernel_image_phy_end_address
.extern find_tag
.extern print_text

.section .data
_mmap_tag_not_found_str:
	.asciz "MULTIBOOT_TAG_TYPE_MMAP not found\r\n"
_avilable_mem_not_found_str:
	.asciz "Avilable memory not found\r\n"


.section .text
.globl init_vmem
.type  init_vmem, @function

# ダイレクトマップおよびカーネルコードを含むリニアアドレスを生成します
# arg0 multiboot2_info_table
init_vmem:
	push %ebx;
	push %edi;
	#allocate stack value
	sub $0x08,%esp;


	#calc memory size
	push 0x0c(%esp);
	call calc_memory_size;
	add $0x04,%esp;
	
	#check ret
	test %eax,%eax;
	jne calc_success;
	test %edx,%edx;
	jne calc_success;

	#failed
	jmp return_init_vm_failed;
calc_success:
	mov %eax,(%esp);
	mov %edx,0x4(%esp);

	mov %eax,(_phy_memory_end);
	mov %edx,(_phy_memory_end + 0x4);

	mov $CPUID_GET_HIGHEST_EXTENDED_FUNCTION, %eax;
	cpuid;
	cmp $CPUID_GET_EXTENDED_PROCESSOR_INFO, %eax;
	jge check_pdpe1gb;
create_page:
	#set cr3
	mov (_kernel_image_phy_end_address),%edx;
	mov %edx,%cr3;

	#fill zero
	mov $(PAGE_SIZE >> 2), %ecx; 
	mov %edx,%edi;
	xor %eax, %eax;
	rep stosl;

	#call create vtable
	push 0x04(%esp);
	push 0x04(%esp);
	push $0;
	push $0;
	push $0;
	push $0;
	push $0;
	push $(PE_PRESENT | PE_RW | PE_US);
	push %edx;
	add $PAGE_SIZE, %edx;
	push %edx;
	call create_vtable;
	add $0x28, %esp;

return_init_vm_success:#カーネル用のページングを行うために最後に確保したページ用メモリの次のアドレスをeaxに格納する必要があります
	#calc kernel code size (by page)
	mov %cr3, %edx;

	#call create vtable
	push $0;
	pushl (_kernel_image_phy_end_address);
	push $0;
	pushl (_kernel_image_phy_start_address);
	pushl $(MEMORY_KERNEL_CODE_SPACE >> 32);
	pushl $MEMORY_KERNEL_CODE_SPACE;
	push $0;
	push $(PE_PRESENT | PE_RW | PE_US | PE_GLOBAL);
	push %edx;
	push %eax;
	call create_vtable;
	add $0x28, %esp;

	//ret
	add $0x08,%esp;
	pop %edi;
	pop %ebx;
	xor %eax,%eax;
	ret;
return_init_vm_failed:
	add $0x20,%esp;
	pop %edi;
	pop %ebx;
	mov $-1,%eax;
	ret;

check_pdpe1gb:
	mov $CPUID_GET_EXTENDED_PROCESSOR_INFO, %eax;
	cpuid;
	test $1 << CPUID_EX_EDX_PAGE1GB_SHIFT, %edx;
	je check_pse;
create_page_pdpe1gb:
	#set cr4
	mov %cr4, %eax;
	or $(CR4_PSE), %eax;
	mov %eax, %cr4;

	#set cr3
	mov (_kernel_image_phy_end_address),%edx;
	mov %edx,%cr3;

	#fill zero
	mov $(PAGE_SIZE >> 2), %ecx; 
	mov %edx,%edi;
	xor %eax, %eax;
	rep stosl;

	#call create vtable
	push 0x04(%esp);
	push 0x04(%esp);
	push $0;
	push $0;
	push $0;
	push $0;
	push $0;
	push $(PE_PRESENT | PE_RW | PE_US | PE_PAT);
	push %edx;
	add $PAGE_SIZE, %edx;
	push %edx;
	call create_vtable_with_pse;
	add $0x28, %esp;

	jmp return_init_vm_success;

check_pse:
	mov $CPUID_GET_PROCESSOR_INFO_AND_FEATURES, %eax;
	cpuid;
	test $1 << CPUID_FEAT_EDX_PSE_SHIFT, %edx;
	je create_page;
create_page_pse:
	#set cr4
	mov %cr4, %eax;
	or $(CR4_PSE), %eax;
	mov %eax, %cr4;

	#set cr3
	mov (_kernel_image_phy_end_address),%edx;
	mov %edx,%cr3;

	#fill zero
	mov $(PAGE_SIZE >> 2), %ecx; 
	mov %edx,%edi;
	xor %eax, %eax;
	rep stosl;

	#call create vtable
	push 0x04(%esp);
	push 0x04(%esp);
	push $0;
	push $0;
	push $0;
	push $0;
	push $0;
	push $(PE_PRESENT | PE_RW | PE_US | PE_PAT);
	push %edx;
	add $PAGE_SIZE, %edx;
	push %edx;
	call create_vtable_with_pse;
	add $0x28, %esp;

	jmp return_init_vm_success;

# arg0 multiboot2_info_table
# ret 64bit address
calc_memory_size:
	push $MULTIBOOT_TAG_TYPE_MMAP;
	push 0x08(%esp);
	call find_tag;
	add $0x08,%esp;
	
	test %eax,%eax;
	je failed_get_mmap_tag;

	mov 0x04(%eax),%ecx;
	mov %eax,%edx;
	add %ecx,%edx;
	
	#offset 範囲外を指すことになるがすぐに0x18を引くので問題ない　memtypeを指すようにするため
	add $0x10,%edx;

find_avilable_mem:
	sub $0x18,%edx;

	cmp %eax,%edx;
	jle failed_get_avilavle_mem;

	cmpl $1, (%edx);
	jne find_avilable_mem;


	#-if find
	sub $0x10,%edx;

	#add low byte
	mov (%edx), %ecx;
	mov 0x08(%edx), %eax;
	add %eax,%ecx;

	#add high byte
	mov 0x04(%edx), %eax;
	adc 0x0c(%edx), %eax;

	#check value (max 64 TiB)
	push %eax;
	push %ecx;

	test %ecx,%ecx;
	je find_avilable_mem_0;
	inc %eax;
find_avilable_mem_0:
	cmp $(0x80 << (PML4_INDEX_SHIFT - 32)),%eax; 
	jle find_avilable_mem_1;
	movl $0, (%esp);
	movl $(0x80 << (PML4_INDEX_SHIFT - 32)), 0x04(%esp);
find_avilable_mem_1:

	pop %eax;
	pop %edx;
	ret;
failed_get_mmap_tag:
	#print failed reason
	push $0;
	push $1;
	push $_mmap_tag_not_found_str;
	call print_text;
	add $0x0c,%esp;

	xor %eax,%eax;
	xor %edx,%edx;
	ret;
failed_get_avilavle_mem:
	#print failed reason
	push $0;
	push $1;
	push $_avilable_mem_not_found_str;
	call print_text;
	add $0x0c,%esp;

	xor %eax,%eax;
	xor %edx,%edx;
	ret;

