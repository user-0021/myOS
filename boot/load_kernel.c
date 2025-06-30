#include <stdint.h>
#include <stddef.h>
#include <multiboot2.h>
#include <multiboot2_utils.h>
#include <page_controller.h>

#ifdef KERNEL_NONCOMPRESS
int32_t load_kernel_nonconpress(char* kernel_code,uint32_t code_size,char* load_address,char* load_address_limit);
#endif

int32_t load_kernel(char* kernel_code,uint32_t code_size,char* info_table){
	//calc size
	uint32_t info_table_size = ((uint32_t*)info_table)[0];
	char* info_table_end = info_table + info_table_size;

	//find Basic memory information
	struct multiboot_tag_basic_meminfo* meminfo;
	meminfo = (void*)find_tag((uint32_t)info_table,MULTIBOOT_TAG_TYPE_BASIC_MEMINFO);
	if(meminfo == NULL)
		return -1;
	
	//calc kernel load address
	char* kernel_load_address = (void*)MULTIBOOT_MEMORY_UPPER_HEAD;
	char* kernel_load_address_limit = (void*)(MULTIBOOT_MEMORY_UPPER_HEAD + (meminfo->mem_upper << 10));
	if(((uint32_t)info_table_end) > MULTIBOOT_MEMORY_UPPER_HEAD){
		kernel_load_address = (void*)(((uint32_t)(info_table_end + MEMORY_PAGE_OFFSET_MASK)) & (~MEMORY_PAGE_OFFSET_MASK));
	}

	#ifdef KERNEL_NONCOMPRESS
	int32_t status = load_kernel_nonconpress(kernel_code,code_size,kernel_load_address,kernel_load_address_limit);
	#endif

	while(1);
	return 0;
}


#ifdef KERNEL_NONCOMPRESS
#endif
int32_t load_kernel_nonconpress(char* kernel_code,uint32_t code_size,char* load_address,char* load_address_limit){
	if(load_address + code_size > load_address_limit)
		return -1;

	uint32_t i;
	for(i = 0,code_size >>= 2;i < code_size;i++){
		((uint32_t*)load_address)[i] = ((uint32_t*)kernel_code)[i];
	}
	
	return 0;
}