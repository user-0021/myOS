.section .kernel_header

header_start:
	.quad _kernel_start
	.quad kernel_gdt_ptr
	.align 8
header_end:

.section .data
.global kernel_gdt_table
.global kernel_gdt_ptr
kernel_gdt_table:
	.long kernel_gdt_table_end - kernel_gdt_table
	.quad 0x0000000000000000
	.quad 0x00af9a000000ffff //64bit-kernel-code
	.quad 0x00cf93000000ffff //64bit-kernel-data
	.quad 0x00affa000000ffff //64bit-user-code
	.quad 0x00cff3000000ffff //64bit-user-data
	.quad 0x00cf9a000000ffff //32bit-kernel-code
	.quad 0x00cf93000000ffff //32bit-kernel-data
	.align 8
kernel_gdt_table_end:

kernel_gdt_ptr:
	.word kernel_gdt_table_end - kernel_gdt_table - 1
	.quad kernel_gdt_table
	.align 8
kernel_gdt_ptr_end: