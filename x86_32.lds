OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386:i386)
ENTRY(_entry_point)


SECTIONS
{
	. = 0x10000;
	_image_start = .;
	. = ALIGN(0x1000);
	.text :
	{
		_text = .;
		*(.multiboot_header)
		*(.text)
		*(.text.*)
		*(.gnu.linkonce.t.*)
		. = ALIGN(16);
	} 
	_etext = .;
	_text_size = . - _text;
	.rdata :
	{
		_data = .;
		*(.got.plt)
		*(.got)
		*(.rodata*)
		*(.srodata)
		*(.gnu.linkonce.r.*)
	}
	. = ALIGN(0x1000);
	.data :
	{
		*(.data*)
		*(.sdata*)
		*(.gnu.linkonce.d.*)
		. = ALIGN(0x1000);
		*(.code64_image*)
	}
	. = ALIGN(0x1000);
	.bss :
	{
		*(.sbss)
		*(.scommon)
		*(.dynbss)
		*(.bss)
		*(.gnu.linkonce.b.*)
		*(COMMON)
	}
	. = ALIGN(0x1000);	
	.stack :
    {
        . = ALIGN(4);
        _stack_start = .;
        . += 0x1000;
        _stack_end = .;
    }
	. = ALIGN(0x1000);
	.dynamic  : { *(.dynamic) }
	. = ALIGN(0x1000);
	.hash : { *(.hash) }
	.gnu.hash : { *(.gnu.hash) }
	.note.gnu.build-id :
    {
        *(.note.gnu.build-id)
    }
	. = ALIGN(0x1000);
	.rel :
	{
		*(.rel.data)
		*(.rel.data.*)
		*(.rel.got)
		*(.rel.stab)
	}
	_edata = .;
	_data_size = . - _etext;
	. = ALIGN(0x1000);
	.reloc :
	{
		LONG(_data);
		LONG(10);
		SHORT(0);
		*(.reloc)
	}
	. = ALIGN(0x1000);
	.dynsym   : { *(.dynsym) }
	. = ALIGN(0x1000);
	.dynstr   : { *(.dynstr) }
	. = ALIGN(0x1000);
	/DISCARD/ :
	{
		*(.rel.reloc)
		*(.eh_frame)
		*(.note*)
		*(.comment*)
	}
	_image_end = .;
}