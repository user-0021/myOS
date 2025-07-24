#include <boot/multiboot2.h>
#include <arch/x86/register.h>
#include <mem/memorymap.h>


.extern _binary_code64_elf_bin_start
.extern _binary_code64_elf_bin_end
.extern load_kernel
.extern init_simple_cui
.extern init_vmem
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
framebuffer_tag_start:
    .word MULTIBOOT_HEADER_TAG_FRAMEBUFFER # type
    .word 0    # flags
    .long framebuffer_tag_end - framebuffer_tag_start # size
    .long 800 # entry
    .long 450 # entry
    .long 24 # entry
	.align  MULTIBOOT_HEADER_ALIGN
framebuffer_tag_end:


    # required end tag
end_tag_start:
    .word MULTIBOOT_TAG_TYPE_END # type
    .word 0    # flags
    .long end_tag_end - end_tag_start # size
	.align  MULTIBOOT_HEADER_ALIGN
end_tag_end:
header_end:
  
# global data
.section .bss
.global _kernel_image_phy_start_address
.global _kernel_image_phy_end_address
.global _phy_memory_end
_kernel_image_phy_start_address: # init in load_kernel and use in init_vmemory 
	.long 0
_kernel_image_phy_end_address: # init in load_kernel and use in init_vmemory 
	.long 0
_phy_memory_end: # init in init_vmemory and pass to kernel
	.quad 0

.section .data
tmp_gdt_table:
	.long tmp_gdt_table_end - tmp_gdt_table
	.quad 0x0000000000000000
	.quad 0x0000ffff00af9a00 //64bit-kernel-code
	.quad 0x0000ffff00cf9300 //64bit-kernel-data
	.align 8
tmp_gdt_table_end:

tmp_gdt_ptr:
	.word tmp_gdt_table_end - tmp_gdt_table - 1
	.long tmp_gdt_table
	.align 8
tmp_gdt_ptr_end:

.section .text
.globl	_entry_point
.type	_entry_point, @function

# entry point
_entry_point:
	#set stack
	mov $(_stack_end), %esp;

	#call init_simple_cui
	push %ebx;
	call init_simple_cui;
	add $0x04,%esp;

	#calc 64bit image address
 	mov $_binary_code64_elf_bin_start, %ecx;
 	mov $_binary_code64_elf_bin_end, %edx;
	sub %ecx, %edx;

	#call load kernel
	push %ebx;
	push %edx;
	push %ecx;
	call load_kernel;
	add $0x0c,%esp;

	#check return
	cmp $0, %eax;
	jl crash;

	#init virtual memory
	push %ebx;
	call init_vmem;
	add $0x04,%esp;

	#check return
	cmp $0, %eax;
	jl crash;
	

	#activeate pageing
	mov %cr4, %eax;
	or $(CR4_PAE), %eax;
	mov %eax, %cr4;

	mov  $(MSR_EFER), %ecx;
	rdmsr;
	or  $(MSR_EFER_LME), %eax;
	wrmsr;

	mov %cr0, %eax;
	or $(CR0_PE | CR0_PG), %eax;
	mov %eax, %cr0;

	#load tmp gdt table
	lgdt (tmp_gdt_ptr);
	mov $0x10,%ax;
	mov %ax, %ds;
	mov %ax, %es; 
	mov %ax, %fs; 
	mov %ax, %gs;
	mov %ax, %ss;

	#push kernel addres
	pushl $(MEMORY_KERNEL_CODE_SPACE >> 32);
	pushl $(MEMORY_KERNEL_CODE_SPACE);

	#push data for far ret
	mov (_kernel_image_phy_start_address), %edx;
	push $0x08;
	pushl $jmp_to_kernel;
	retf;


crash:
	cli;
    hlt;

jmp_to_kernel:
	ret;
