#include <stdint.h>
#include <stddef.h>
#include <multiboot2/multiboot2.h>

void failed_start_up(){
	while(1);
}


/**
 * @brief kernel entry point
 * 
 * @param multiboot2_info_table  multiboot2 info table 
 * @param page free address space head 
 * @return int 
 */
int _init_kernel(uint8_t* multiboot2_info_table,uint8_t* page){
	
	//info
	uint64_t memory_size = 0;
	struct multiboot_tag_mmap* mmap_entry = NULL;

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

	//error check
	if(mmap_entry == NULL){
		failed_start_up();
	}
	
	//calc memory size
	int32_t entry_count = (mmap_entry->size - 0x10) / mmap_entry->entry_size;
	for(;0 < entry_count;){
		entry_count--;
		if(mmap_entry->entries[entry_count].type == MULTIBOOT_MEMORY_AVAILABLE){
			memory_size = mmap_entry->entries[entry_count].addr + mmap_entry->entries[entry_count].len;
		}
	}

	//error check
	if(memory_size == 0){
		failed_start_up();
	}

	
}
