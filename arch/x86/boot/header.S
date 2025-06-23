.extern _binary_code64_elf_bin_start
.extern _binary_code64_elf_bin_end
.extern _load_kernelCode

.section .multiboot_header
header_start:
    .long 0xe85250d6                 # magic number (multiboot 2)
    .long 0                          # architecture 0 (protected mode i386)
    .long header_end - header_start  # header length
    # checksum
    .long 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    # insert optional multiboot tags here

    # required end tag
    .word 0    # type
    .word 0    # flags
    .long 8    # size
header_end:

.section .data
.globl _init_stack         
.type _init_stack, @object 
.globl _init_stack_end         
.type _init_stack_end, @object 
.align 16                      
_init_stack:
	.space 4096                
_init_stack_end:


.section .text
.globl	_entry_point
.type	_entry_point, @function


_entry_point:
	mov $_init_stack_end, %esp;
	mov $_binary_code64_elf_bin_start, %ecx;
	mov $_binary_code64_elf_bin_end, %edx;
	sub %ecx, %edx;

	call _load_kernelCode;

    nop;
    nop;
    nop;
    hlt;
	jmp _entry_point;