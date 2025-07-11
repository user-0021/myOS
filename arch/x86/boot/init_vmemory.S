#include <boot/multiboot2.h>
#include <arch/x86/cpuid.h>
#include <arch/x86/page_define.h>

.extern _kernel_image_phy_start_address
.extern _kernel_image_phy_end_address
.extern find_tag
.extern print_text

.section .data
mmap_tag_not_found_str:
	.asciz "MULTIBOOT_TAG_TYPE_MMAP not found\r\n"

.section .text
.globl init_vmem
.type  init_vmem, @function

# arg0 multiboot2_info_table
init_vmem:
	push 0x4(%esp);
	call calc_memory_size;
	add $0x04,%esp;
	
	test %eax,%eax;
	jne calc_success;
	
	test %edx,%edx;
	jne calc_success;

	jmp return_init_vm_failed;
calc_success:
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
	mov $0,%eax;
	ret;
return_init_vm_failed:
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
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
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
	je calc_failed;

	mov 0x04(%eax),%ecx;
	mov %eax,%edx;
	add %ecx,%edx;

find_avilable_mem:
	sub $0x18,%edx;

	cmp $1, %edx;
	jne find_avilable_mem:

	hlt;
	ret;
calc_failed:
	#print failed reason
	push $0;
	push $1;
	push $mmap_tag_not_found_str;
	call print_text;
	add $0x0c,%esp;

	mov $0,%eax;
	mov $0,%edx;
	ret;

