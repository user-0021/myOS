#include <stdint.h>
#include <multiboot2.h>

int _init_kernel(uint8_t* multiboot2_info_table,uint8_t* page){
	//info
	struct multiboot_tag_mmap* mmap_entry;

	//calc size
	uint32_t info_table_size = ((uint32_t*)multiboot2_info_table)[0];
	uint8_t* info_table_end = multiboot2_info_table + info_table_size;

	//find need information
	multiboot2_info_table += 8;
	while (multiboot2_info_table < info_table_end)
	{
		switch (((uint32_t*)multiboot2_info_table)[0])
		{
		case MULTIBOOT_TAG_TYPE_MMAP:
			mmap_entry = multiboot2_info_table;
			break;
		
		default:
			break;
		}

		multiboot2_info_table += (((uint32_t*)multiboot2_info_table)[1] + MULTIBOOT_INFO_ALIGN - 1) & (~(MULTIBOOT_INFO_ALIGN - 1));
	}

	
	//
}
