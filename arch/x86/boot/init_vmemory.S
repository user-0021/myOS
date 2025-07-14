#include <boot/multiboot2.h>
#include <arch/x86/cpuid.h>
#include <arch/x86/page_define.h>

.extern _kernel_image_phy_start_address
.extern _kernel_image_phy_end_address
.extern find_tag
.extern print_text

.section .data
.global _phy_memory_end;
.global _page_phy_start_address
.global _page_lev4_count
.global _page_lev3_count
.global _page_lev2_count
.global _page_lev1_count
_phy_memory_end:
	.quad 0
_page_phy_start_address:
	.long 0
_page_lev4_count:
	.long 0
_page_lev3_count:
	.long 0
_page_lev2_count:
	.long 0
_page_lev1_count:
	.long 0
_mmap_tag_not_found_str:
	.asciz "MULTIBOOT_TAG_TYPE_MMAP not found\r\n"
_avilable_mem_not_found_str:
	.asciz "Avilable memory not found\r\n"


.section .text
.globl init_vmem
.type  init_vmem, @function

# arg0 multiboot2_info_table
init_vmem:
	#allocate stack value
	sub $0x08,%esp;


	#calc memory size
	push 0xc(%esp);
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
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
return_init_vm_success:
	add $0x08,%esp;
	mov $0,%eax;
	ret;
return_init_vm_failed:
	add $0x08,%esp;
	mov $-1,%eax;
	ret;

check_pdpe1gb:
	mov $CPUID_GET_EXTENDED_PROCESSOR_INFO, %eax;
	cpuid;
	test $1 << CPUID_EX_EDX_PAGE1GB_SHIFT, %edx;
	je check_pse;
create_page_pdpe1gb:
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	jmp return_init_vm_success;

check_pse:
	mov $CPUID_GET_PROCESSOR_INFO_AND_FEATURES, %eax;
	cpuid;
	test $1 << CPUID_FEAT_EDX_PSE_SHIFT, %edx;
	je create_page;
create_page_pse:

	#calc need page count (lev4pages + lev3pages + lev2pages)
	push $1; # lev4 page count
	mov $1,(_page_lev4_count);

	#add page3 page count
	mov 0x08(%esp), %eax;
	shr $(PML4_INDEX_SHIFT - 32),%eax;
	and $0x1FF, %eax;
	inc %eax;
	add %eax, (%esp); 
	mov %eax,(_page_lev3_count);

	#add page2 page count
	mov 0x08(%esp), %edx;
	shl $(32 - PML3_INDEX_SHIFT),%edx;
	and $0x1FF, %edx;

	mov 0x04(%esp), %ecx;
	shr $(PML3_INDEX_SHIFT),%ecx;
	and $0x1FF, %ecx;
	inc %ecx;
	add %ecx, %edx;

	shl $0x09, %eax;
	sub $0x200, %eax;
	add %edx, %eax;

	add %eax, (%esp); 
	mov %eax,(_page_lev2_count);

	#get need page counts
	pop %eax;
	
	mov (_kernel_image_phy_end_address),%edx;


	hlt;
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

	cmp $1, (%edx);
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

	#set ret value
	mov %ecx, %edx;
	xchg %edx,%eax;

	ret;
failed_get_mmap_tag:
	#print failed reason
	push $0;
	push $1;
	push $_mmap_tag_not_found_str;
	call print_text;
	add $0x0c,%esp;

	mov $0,%eax;
	mov $0,%edx;
	ret;
failed_get_avilavle_mem:
	#print failed reason
	push $0;
	push $1;
	push $_avilable_mem_not_found_str;
	call print_text;
	add $0x0c,%esp;

	mov $0,%eax;
	mov $0,%edx;
	ret;

