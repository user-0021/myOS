.extern _binary_code64_elf_bin_start
.extern _binary_code64_elf_bin_end
.extern _load_kernelCode

#HEAD
.equ MULTIBOOT_MAGIC, 0xe85250d6
.equ MULTIBOOT_ARCH, 0
#TAG TYPES
.equ END_TAG_TYPE, 0
.equ ADDRESS_TAG_TYPE, 2
.equ ENTRY_ADDRESS_TAG_TYPE, 3
.equ MULTIBOOT_ARCH, 0
#DEFINE
.equ STACK_SIZE, 4096

.section .multiboot_header
.align  8
header_start:
    .long MULTIBOOT_MAGIC                 # magic number (multiboot 2)
    .long MULTIBOOT_ARCH             # architecture 0 (protected mode i386)
    .long header_end - header_start  # header length
    # checksum
    .long 0x100000000 - (MULTIBOOT_MAGIC + MULTIBOOT_ARCH + (header_end - header_start))

    # insert optional multiboot tags here

    # entry address tag
entry_address_tag_start:
    .word ENTRY_ADDRESS_TAG_TYPE # type
    .word 0    # flags
    .long entry_address_tag_end - entry_address_tag_start # size
    .long _entry_point # entry
	.align  8
entry_address_tag_end:

    # required end tag
end_tag_start:
    .word END_TAG_TYPE # type
    .word 0    # flags
    .long end_tag_end - end_tag_start # size
end_tag_end:
header_end:

.section .data
.align 16         
.globl _init_stack_begin 
_init_stack_begin:
    .space STACK_SIZE        

.section .text
.globl	_entry_point
.type	_entry_point, @function


_entry_point:
	mov $(_init_stack_begin + STACK_SIZE), %esp;
 	mov $_binary_code64_elf_bin_start, %ecx;
 	mov $_binary_code64_elf_bin_end, %edx;
	sub %ecx, %edx;

	call _load_kernelCode;

    nop;
    nop;
    nop;
    hlt;
	jmp _entry_point;