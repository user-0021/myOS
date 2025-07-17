#include <boot/multiboot2.h>
#include <arch/x86/cpuid.h>
#include <arch/x86/page_define.h>

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
return_init_vm_success:#カーネル用のページングを行うために最後に確保したページ用メモリの次のアドレスをeaxに格納する必要があります
	#calc kernel code size (by page)
	mov (_kernel_image_phy_end_address), %ecx;
	sub (_kernel_image_phy_start_address), %ecx;
	push %ecx;

	#calc lev1 page counts
	add $PAGE_OFFSET_MASK, %ecx;
	shr $PAGE_SIZE_SHIFT, %ecx;
	push %ecx;

	#calc lev2 page counts
	add $(PT_ENTRY_COUNT - 1), %ecx;
	shr $PT_INDEX_BITS, %ecx;
	push %ecx;

	#calc lev3 page counts
	add $(PT_ENTRY_COUNT - 1), %ecx;
	shr $PT_INDEX_BITS, %ecx;
	push %ecx;

	hlt;

	# init pages
	# init for loop
	mov %cr3, %edx;
	add ,%edx;
kcode_lev3_page_init_loop:
	#check loop
	test %ecx,%ecx;
	je kcode_lev3_page_init_loop_end;
	dec %ecx;
	#init lev3 page
	movl $(PE_PRESENT | PE_RW | PE_US) , (%edi);
	or  %edx, (%edi);#32bit アドレス空間でやっているので、上位アドレスは設定しなくて良い
	#inc itr
	mov %edx, %ebx;
	add $0x08, %edi;
	add $PAGE_SIZE, %edx;
	#calc need lev2 page size
	mov 0x08(%esp),%eax;
	movl $0, 0x08(%esp);
	cmp $0x200, %eax;
	jle kcode_lev2_page_init_loop;
	sub $0x200, %eax;
	mov %eax,0x08(%esp);
	mov $0x200, %eax;
kcode_lev2_page_init_loop:
	#check loop
	test %eax,%eax;
	je kcode_lev2_page_init_loop_end;
	dec %eax;
	#init lev2 page
	movl $(PE_PRESENT | PE_RW | PE_US), (%ebx);
	or  %edx, (%ebx);#32bit アドレス空間でやっているので、上位アドレスは設定しなくて良い

	push %eax;
kcode_write_page_addr:
	#set phy address to phy address map 
	mov 0x04(%esp), %eax;
	shl $PML2_INDEX_SHIFT,%eax;
	movl $(PE_PRESENT | PE_RW | PE_US | PE_PAT | PE_GLOBAL), (%edx);
	or  %eax, (%edx);
	mov 0x04(%esp), %eax;
	shr $(32 - PML2_INDEX_SHIFT), %eax;
	movl $((PE_NX) >> 32), 0x04(%edx);
	or  %eax, 0x04(%edx);
	add $0x08, %edx;
	mov 0x04(%esp), %eax;
	inc %eax;
	mov %eax,0x04(%esp);
	#loop check
	testl $PAGE_OFFSET_MASK,%edx;
	jne kcode_write_page_addr;
	pop %eax;
	

	#inc itr
	add $0x08, %ebx;
	//add $PAGE_SIZE, %edx;

	jmp kcode_lev2_page_init_loop;
kcode_lev2_page_init_loop_end:


	jmp kcode_lev3_page_init_loop;
kcode_lev3_page_init_loop_end:


	


	add $0x08,%esp;
	pop %edi;
	pop %ebx;
	xor %eax,%eax;
	ret;
return_init_vm_failed:
	add $0x08,%esp;
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
	#set cr3
	mov (_kernel_image_phy_end_address),%edx;
	mov %edx,%cr3;

	#get max address
	mov (%esp), %ecx;
	mov 0x04(%esp), %eax;

	#calc lev1 page counts
	//add $PAGE_OFFSET_MASK, %ecx;
	//shr $PAGE_SIZE_SHIFT, %ecx;
	//push %ecx;

	#calc lev2 page counts
	add $(((PT_ENTRY_COUNT - 1) << PAGE_SIZE_SHIFT) | PAGE_OFFSET_MASK), %ecx;
	shr $(PT_INDEX_BITS + PAGE_SIZE_SHIFT), %ecx;
	push %ecx;

	#calc lev3 page counts
	add $(PT_ENTRY_COUNT - 1), %ecx;
	shr $PT_INDEX_BITS, %ecx;
	push %ecx;

	hlt;
	#fill zero
	shl $(3 << (PAGE_SIZE_SHIFT - 2)),%ecx;# (1 lev4_pages + 1 lev3_pages + 1 lev2_pages) << (page_shift - long size) 
	mov %edx,%edi;
	xor %eax, %eax;
	rep stosl;

	//init lev4 page
	mov %edx, %eax;
	addl $PAGE_SIZE, %eax;
	movl $(PE_PRESENT | PE_RW | PE_US) , (%edx);
	or  %eax, (%edx);#32bit アドレス空間でやっているので、上位アドレスは設定しなくて良い

	//init lev3 page
	mov %eax, %edx;
	addl $PAGE_SIZE, %edx;
	movl $(PE_PRESENT | PE_RW | PE_US) , (%eax);
	or  %edx, (%eax);#32bit アドレス空間でやっているので、上位アドレスは設定しなくて良い

	//init lev2 page
	mov %edx, %eax;
	xor %edx,%edx;
pse_lev2_page_init_loop:
	#set phy address to phy address map
	shl $PML2_INDEX_SHIFT,%eax;
	movl $(PE_PRESENT | PE_RW | PE_US | PE_PAT | PE_GLOBAL), (%eax);
	or  %edx, (%eax);
	movl $((PE_NX) >> 32), 0x04(%eax);

	add $0x08, %edx;
	mov 0x04(%esp), %eax;
	inc %eax;
	mov %eax,0x04(%esp);
	#loop check
	testl $PAGE_OFFSET_MASK,%edx;
	jne pse_lev2_page_init_loop;
pse_lev2_page_init_loop_end:

	# init pages
	# init for loop
	mov %edx, %edi;
	add $PAGE_SIZE, %edx;
	mov 0x04(%esp),%ecx;
	movl $0x00,(%esp);
pse_lev3_page_init_loop:
	#check loop
	test %ecx,%ecx;
	je pse_lev3_page_init_loop_end;
	dec %ecx;
	#inc itr
	mov %edx, %ebx;
	add $0x08, %edi;
	add $PAGE_SIZE, %edx;
	#calc need lev2 page size
	mov 0x08(%esp),%eax;
	movl $0, 0x08(%esp);
	cmp $0x200, %eax;
	jle pse_lev2_page_init_loop;
	sub $0x200, %eax;
	mov %eax,0x08(%esp);
	mov $0x200, %eax;
pse_lev2_page_init_loop:
	#check loop
	test %eax,%eax;
	je pse_lev2_page_init_loop_end;
	dec %eax;
	#init lev2 page
	movl $(PE_PRESENT | PE_RW | PE_US), (%ebx);
	or  %edx, (%ebx);#32bit アドレス空間でやっているので、上位アドレスは設定しなくて良い

	push %eax;
pse_write_page_addr:
	#set phy address to phy address map 
	mov 0x04(%esp), %eax;
	shl $PML2_INDEX_SHIFT,%eax;
	movl $(PE_PRESENT | PE_RW | PE_US | PE_PAT | PE_GLOBAL), (%edx);
	or  %eax, (%edx);
	mov 0x04(%esp), %eax;
	shr $(32 - PML2_INDEX_SHIFT), %eax;
	movl $((PE_NX) >> 32), 0x04(%edx);
	or  %eax, 0x04(%edx);
	add $0x08, %edx;
	mov 0x04(%esp), %eax;
	inc %eax;
	mov %eax,0x04(%esp);
	#loop check
	testl $PAGE_OFFSET_MASK,%edx;
	jne pse_write_page_addr;
	pop %eax;
	

	#inc itr
	add $0x08, %ebx;
	//add $PAGE_SIZE, %edx;

	jmp pse_lev2_page_init_loop;
pse_lev2_page_init_loop_end:


	jmp pse_lev3_page_init_loop;
pse_lev3_page_init_loop_end:
	add $0x0c,%esp;
	mov %edx, %eax;
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

