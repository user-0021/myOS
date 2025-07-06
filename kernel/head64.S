.section .kernel_header

header_start:
	.quad _kernel_start
header_end:

.section .data
.global kernel_image_phy_start_address
kernel_image_phy_start_address:
	.long 0