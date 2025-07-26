#include <stdint.h>
#include <stddef.h>
#include <multiboot2/multiboot2.h>
#include <boot/multiboot2_utils.h>
#include <boot/simple_cui.h>
#include <arch/x86/page_define.h>

extern uint32_t _kernel_image_phy_start_address;
extern uint32_t _kernel_image_phy_end_address;

#ifdef KERNEL_NONCOMPRESS
int32_t _load_kernel_nonconpress(char* kernel_code,uint32_t code_size,char* load_address,char* load_address_limit);
#endif

/**
 * @brief load 64bit kernel image on physical address
 * 
 * @param kernel_code start address of kernel image 
 * @param code_size   kernel image size
 * @param info_table  multiboot2 info tag table
 * @return int32_t status code
 */
uint32_t load_kernel(char* kernel_code,uint32_t code_size,char* info_table){
	//calc size
	uint32_t info_table_size = ((uint32_t*)info_table)[0];
	char* info_table_end = info_table + info_table_size;
	
	//find Basic memory information
	struct multiboot_tag_basic_meminfo* meminfo;
	meminfo = (void*)find_tag((uint32_t)info_table,MULTIBOOT_TAG_TYPE_BASIC_MEMINFO);
	if(meminfo == NULL){
		print_text("MULTIBOOT_BASIC_MEMINFO not found.",SIMPLE_CUI_WHITE,SIMPLE_CUI_BLACK);
		return -1;
	}
	
	//calc kernel load address
	char* kernel_load_address = (void*)MULTIBOOT_MEMORY_UPPER_HEAD;
	char* kernel_load_address_limit = (void*)(MULTIBOOT_MEMORY_UPPER_HEAD + (meminfo->mem_upper << 10));
	if(((uint32_t)info_table_end) > MULTIBOOT_MEMORY_UPPER_HEAD){
		kernel_load_address = (void*)(uint32_t)(((uint32_t)info_table_end + PAGE_OFFSET_MASK) & (~PAGE_OFFSET_MASK));
	}

	//load
	int32_t status;
	#ifdef KERNEL_NONCOMPRESS
	status = _load_kernel_nonconpress(kernel_code,code_size,kernel_load_address,kernel_load_address_limit);
	#endif

	//check
	if(status < 0){
		print_text("Failed load kernel code.",SIMPLE_CUI_WHITE,SIMPLE_CUI_BLACK);
		return -1;
	}

	return 0;
}


#ifdef KERNEL_NONCOMPRESS
/**
 * @brief load noncompress image
 * 
 * @param kernel_code        start address of kernel image 
 * @param code_size          kernel image size
 * @param load_address       dist address
 * @param load_address_limit load address limit
 * @return int32_t status code
 */
int32_t _load_kernel_nonconpress(char* kernel_code,uint32_t code_size,char* load_address,char* load_address_limit){
	if(load_address + code_size > load_address_limit)
		return -1;

	uint32_t i;
	uint32_t count = code_size >> 2;
	for(i = 0;i < count;i++){
		((uint32_t*)load_address)[i] = ((uint32_t*)kernel_code)[i];
	}
	
	_kernel_image_phy_start_address = (uint32_t)load_address;
	_kernel_image_phy_end_address = ((uint32_t)load_address + code_size + PAGE_OFFSET_MASK) & (~PAGE_OFFSET_MASK);
	
	return 0;
}
#endif