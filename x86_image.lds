OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_data)



SECTIONS
{
	. = 0x10000;
	.data : 
	{
		_data = .;
		_image_head = .;
		*(.data*)
		*(.note.gnu.build-id*)
	}
	. = ALIGN(0x1000);

	.hash : { *(.hash) }
	.gnu.hash : { *(.gnu.hash) }
	.note.gnu.build-id :
    {
        *(.note.gnu.build-id)
    }
	. = ALIGN(0x1000);

	/DISCARD/ :
	{
		*(.hash)
		*(.rel.reloc)
		*(.rel.reloc)
		*(.eh_frame)
		*(.note*)
		*(.comment*)
	}
	_image_end = .;
}