/**
 * @file create_vtable.h
 * @author user-0021
 * @brief x86_64アーキテクチャの仮想ページを生成するための関数など
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#ifdef __ASSEMBLER__
.extern create_vtable_with
.extern create_vtable_with_pse
#else
#include <stdint.h>
/**
 * @brief Create a vtable object
 * 
 * @param [in] page_buffer page table buffer (need enough size)
 * @param [in] lev4_page lev4 page addr
 * @param [in] flag page flag
 * @param [in] v_begin virtual address begin
 * @param [in] phy_begin physycal address begin
 * @param [in] phy_end  physycal address end
 * @return uint64_t* end of buffer address
 * 
 * @ingroup x86_64-32bit
 */
uint64_t* create_vtable(uint64_t* page_buffer,uint64_t* lev4_page,uint64_t flag,uint64_t v_begin,uint64_t phy_begin,uint64_t phy_end);

/**
 * @brief Create a vtable object by megabyte-page
 * 
 * @param [in] page_buffer page table buffer (need enough size)
 * @param [in] flag page flag
 * @param [in] v_begin virtual address begin
 * @param [in] phy_begin physycal address begin
 * @param [in] phy_end  physycal address end
 * @return uint64_t* end of buffer address
 * @ingroup x86_64-32bit
 */
uint64_t* create_vtable_with_pse(uint64_t* page_buffer,uint64_t* lev4_page,uint64_t flag,uint64_t v_begin,uint64_t phy_begin,uint64_t phy_end);

/**
 * @brief Create a vtable object by gigabyte-page
 * 
 * @param [in] page_buffer page table buffer (need enough size)
 * @param [in] flag page flag
 * @param [in] v_begin virtual address begin
 * @param [in] phy_begin physycal address begin
 * @param [in] phy_end  physycal address end
 * @return uint64_t* end of buffer address
 * @ingroup x86_64-32bit
 */
uint64_t* create_vtable_with_pdpe1gb(uint64_t* page_buffer,uint64_t* lev4_page,uint64_t flag,uint64_t v_begin,uint64_t phy_begin,uint64_t phy_end);
#endif