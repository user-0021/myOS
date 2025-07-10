#include <arch/x86/cpuid.h>

.extern _kernel_image_phy_start_address
.extern _kernel_image_phy_end_address

.section .text
.globl init_vmem
.type  init_vmem, @function


init_vmem:
# check pdpe1gb support
	mov $CPUID_GET_HIGHEST_EXTENDED_FUNCTION, %eax;
	cpuid;
	cmp $CPUID_GET_EXTENDED_PROCESSOR_INFO, %eax;
	jge check_pdpe1gb;
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
return_init_vm:
	ret;

check_pdpe1gb:
	mov $CPUID_GET_EXTENDED_PROCESSOR_INFO, %eax;
	cpuid;
	shr $CPUID_EX_EDX_PAGE1GB_SHIFT, %edx;
	test $1, %edx;
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
	jmp return_init_vm;

check_pse:
	mov $CPUID_GET_PROCESSOR_INFO_AND_FEATURES, %eax;
	cpuid;
	shr $CPUID_FEAT_EDX_PSE_SHIFT, %edx;
	test $1, %edx;
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
	jmp return_init_vm;


create_page:
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	jmp return_init_vm;