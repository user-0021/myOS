.extern _kernel_image_phy_start_address
.extern _kernel_image_phy_end_address

.section .text
.globl init_vmem
.type  init_vmem, @function


init_vmem:
# check pdpe1gb support
	mov $0x80000000, %eax;
	cpuid;
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
	ret;