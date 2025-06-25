.extern _image_end

.section .text
.globl	_load_kernelCode
.type	_load_kernelCode, @function

#ifdef KERNEL_NONCOMPRESS
# 0 ecx kernel_bin_start
# 1 edx kernel_bin_size
# ret kernel_image_end
_load_kernelCode:
	push %ebx;
	mov %ecx, %eax;
	mov %edx, %ecx;
	add $3, %ecx;
	shr $2, %ecx;
	mov $0x200A, %ebx;
	and $0xFFFFFFFF,%ebx;
	movl $10, (%ebx);
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
	hlt;
L_copy_loop:
	push (%eax);
	pop (%ebx);
	add $4, %eax;
	add $4, %ebx;
	loop L_copy_loop;

	mov %ebx, %eax;
	pop %ebx;
	ret;

#endif