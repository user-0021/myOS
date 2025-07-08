#pragma once
#include <stdint.h>
#include <boot/multiboot2.h>

uint32_t find_tag(uint32_t info_table,uint32_t target_tag_type);
