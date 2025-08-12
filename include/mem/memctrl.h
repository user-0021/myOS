#pragma once
#include <stdint.h>
#include <multiboot2.h>

/**
 * @brief allocate page hub on phy memory and virt memory。
 * @details when alllocate, this function 。
 * 
 * @param [in] tag_mmap multiboot2 memory map tag
 */
void generate_page_hub(struct multiboot_tag_mmap* tag_mmap);