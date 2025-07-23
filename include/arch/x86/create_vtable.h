#pragma once

#ifdef __ASSEMBLER__
.extern create_vtable_with
.extern create_vtable_with_pse
#else
#include <stdint.h>
uint64_t* create_vtable(uint64_t* page_buffer,uint64_t* lev4_page,uint64_t flag,uint64_t v_begin,uint64_t phy_begin,uint64_t phy_end);
uint64_t* create_vtable_with_pse(uint64_t* page_buffer,uint64_t* lev4_page,uint64_t flag,uint64_t v_begin,uint64_t phy_begin,uint64_t phy_end);
#endif