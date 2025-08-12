#pragma once
#include <stdint.h>
#include <multiboot2/multiboot2.h>

/**
 * @brief find tag from multiboot2 info table
 * 
 * @param [in] info_table      multiboot2 info table address
 * @param [in] target_tag_type target tag type
 * @return uint32_t info struct address or NULL if not find.  
 */
uint32_t find_tag(uint32_t info_table,uint32_t target_tag_type);
