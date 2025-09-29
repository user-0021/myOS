#pragma once
#include <stdint.h>
#include <multiboot2.h>

typedef void* PHYSICS_ADDRESS;
typedef void* VIRTUAL_ADDRESS;

/**
 * @brief allocate page hub on phy memory and virt memory。
 * @details when alllocate, this function 。
 * 
 * @param [in] tag_mmap multiboot2 memory map tag
 * @param [in] allocated_space_end allocated address end
 */
void generate_page_hub(struct multiboot_tag_mmap* tag_mmap,PHYSICS_ADDRESS allocated_space_end);

/**
 * @brief allocate page from memory
 * 
 * @param page_count 
 * @param flag 
 * @return PHYSICS_ADDRESS 
 */
PHYSICS_ADDRESS allocate_page(uint64_t page_count,uint64_t flag);