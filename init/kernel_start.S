.section .data
.global kernel_gdt_table
.global kernel_gdt_ptr
kernel_gdt_table:
	.long kernel_gdt_table_end - kernel_gdt_table
	.quad 0x0000000000000000
	.quad 0x0000ffff00af9a00 //64bit-kernel-code
	.quad 0x0000ffff00cf9300 //64bit-kernel-data
	.quad 0x0000ffff00affa00 //64bit-user-code
	.quad 0x0000ffff00cff300 //64bit-user-data
	.quad 0x0000ffff00cf9a00 //32bit-kernel-code
	.quad 0x0000ffff00cf9300 //32bit-kernel-data
	.align 8
kernel_gdt_table_end:

kernel_gdt_ptr:
	.word kernel_gdt_table_end - kernel_gdt_table - 1
	.quad kernel_gdt_table
	.align 8
kernel_gdt_ptr_end:



.extern _init_kernel


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

	#set  arg
	mov $0xFFFFFFFF, %rdi;
	and %rbx,%rdi;
	mov $0xFFFFFFFF, %rsi;
	and %rdx,%rsi;

	#push data for far ret
	push $0x08;
	push $_init_kernel;
	retfq;
