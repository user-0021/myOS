#pragma once
#include <stdint.h>
#include <multiboot2/multiboot2.h>

uint32_t find_tag(uint32_t info_table,uint32_t target_tag_type);
