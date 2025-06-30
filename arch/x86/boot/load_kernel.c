#include <stdint.h>
#include <multiboot2.h>

#ifdef KERNEL_NONCOMPRESS
int32_t load_kernel_nonconpress(char* kernel_code,uint32_t code_size,char* load_address,char* load_address_limit);
#endif

void load_kernel(char* kernel_code,uint32_t code_size,char* tag_table){

	//calc info
	uint32_t tag_table_size = ((uint32_t*)tag_table)[0];
	char* tag_table_end = tag_table + tag_table_size;

	//find Basic memory information
	tag_table += 8;
	while (tag_table < tag_table_end)
	{
		if(((uint32_t*)tag_table)[0] == MULTIBOOT_TAG_TYPE_BASIC_MEMINFO)
			break;

		tag_table += (((uint32_t*)tag_table)[1] + 7) & (~0x7);
	}
	
	//calc kernel load address
	struct multiboot_tag_basic_meminfo* meminfo = (void*)tag_table;
	char* kernel_load_address = (void*)0x100000;
	char* kernel_load_address_limit = (void*)(0x100000 + (meminfo->mem_upper << 10));
	if(((uint32_t)tag_table_end) > 0x100000){
		kernel_load_address = (void*)(((uint32_t)(tag_table_end + 0xFFF)) & (~0xFFF));
	}

	#ifdef KERNEL_NONCOMPRESS
	int32_t status = load_kernel_nonconpress(kernel_code,code_size,kernel_load_address,kernel_load_address_limit);
	#endif

	while(1);
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