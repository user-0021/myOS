#include <stddef.h>
#include <multiboot2/multiboot2.h>
#include <boot/multiboot2_utils.h>

uint32_t find_tag(uint32_t info_table,uint32_t target_tag_type){
	char* info_table_p = (void*)info_table;

	//calc size
	uint32_t info_table_size = ((uint32_t*)info_table_p)[0];
	char* info_table_end = info_table_p + info_table_size;

	//find Basic memory information
	info_table_p += 8;
	while (info_table_p < info_table_end)
	{
		if(((uint32_t*)info_table_p)[0] == target_tag_type)
			return (uint32_t)info_table_p;

		info_table_p += (((uint32_t*)info_table_p)[1] + MULTIBOOT_INFO_ALIGN - 1) & (~(MULTIBOOT_INFO_ALIGN - 1));
	}

	return (uint32_t)NULL;
}
