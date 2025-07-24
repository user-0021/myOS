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

.section .text
.globl	_kernel_start
.type	_kernel_start, @function
_kernel_start:
#need multiboot2 tag table
	#load gdt
	lgdt (kernel_gdt_ptr);
	mov $0x10,%ax;
	mov %ax, %ds;
	mov %ax, %es; 
	mov %ax, %fs; 
	mov %ax, %gs;
	mov %ax, %ss;

	//#push kernel addres
	//pushl $(MEMORY_KERNEL_CODE_SPACE >> 32);
	//pushl $(MEMORY_KERNEL_CODE_SPACE);

	//#push data for far ret
	//mov (_kernel_image_phy_start_address), %edx;
	//push $0x08;
	//pushl $jmp_to_kernel;
	//retf;
	hlt;