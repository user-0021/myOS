.extern _image_end

.section .text
.globl	_load_kernelCode
.type	_load_kernelCode, @function

#ifdef KERNEL_NONCOMPRESS
# 0 ecx kernel_bin_start
# 1 edx kernel_bin_size
# ret kernel_image_end
_load_kernelCode:
	mov %ecx, %eax;
	mov %edx, %ecx;
	mov $2, %cl;
	shr %ecx, %cl;
	mov $_image_end
L_copy_loop:
	//mov (%eax),()
	loop L_copy_loop;

	ret;

#endif