#include <multiboot2.h>

.extern _binary_code64_elf_bin_start
.extern _binary_code64_elf_bin_end
.extern load_kernel
.extern init_simple_cui
.extern _stack_end

.section .multiboot_header
.align  MULTIBOOT_HEADER_ALIGN
header_start:
    .long MULTIBOOT2_HEADER_MAGIC                 
    .long MULTIBOOT_ARCHITECTURE_I386
    .long header_end - header_start 
    # checksum
    .long 0x100000000 - (MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (header_end - header_start))

    # insert optional multiboot tags here

    # entry address tag
entry_address_tag_start:
    .word MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS # type
    .word 0    # flags
    .long entry_address_tag_end - entry_address_tag_start # size
    .long _entry_point # entry
	.align  MULTIBOOT_HEADER_ALIGN
entry_address_tag_end:

    # frame buffer request
//framebuffer_tag_start:
//    .word MULTIBOOT_HEADER_TAG_FRAMEBUFFER # type
//    .word 0    # flags
//    .long framebuffer_tag_end - framebuffer_tag_start # size
//    .long 800 # entry
//    .long 450 # entry
//    .long 24 # entry
//	.align  MULTIBOOT_HEADER_ALIGN
//framebuffer_tag_end:


    # required end tag
end_tag_start:
    .word MULTIBOOT_TAG_TYPE_END # type
    .word 0    # flags
    .long end_tag_end - end_tag_start # size
	.align  MULTIBOOT_HEADER_ALIGN
end_tag_end:
header_end:
  

.section .text
.globl	_entry_point
.type	_entry_point, @function


# entry point
_entry_point:
	#set stack
	mov $(_stack_end), %esp;

	//call init_simple_cui
	push %ebx;
	push %ebx;
	call init_simple_cui;
	pop %ebx;

	#calc 64bit image address
 	mov $_binary_code64_elf_bin_start, %ecx;
 	mov $_binary_code64_elf_bin_end, %edx;
	sub %ecx, %edx;

	#call load kernel
	push %ebx;
	push %edx;
	push %ecx;
	call load_kernel;

    nop;
    nop;
    nop;
    hlt;
    hlt;
    hlt;
    hlt;
	jmp _entry_point;