/**
 * @file create_vtable.c
 * @author user-0021
 * @brief x86_64　アーキテクチャでブート用に仮想メモリを生成するための関数群を含むファイルです
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <stdint.h>
#include <arch/x86/page_define.h>

inline void fill_zero(void* addr,uint32_t len){
    __asm__ volatile (
        "mov %[size], %%ecx\n\t"  
        "mov %[address], %%edi\n\t"
        "xor %%eax, %%eax\n\t"
        "rep stosl\n\t"      
        :                   
        : [address] "r"(addr), [size] "r"(len >> 2) 
        : "ecx", "edi", "eax", "memory"
    );
}

/**
 * @brief Create a vtable object
 * 
 * @param page_buffer page table buffer (need enough size)
 * @param lev4_page lev4 page addr
 * @param flag page flag
 * @param v_begin virtual address begin
 * @param phy_begin physycal address begin
 * @param phy_end  physycal address end
 * @return uint64_t* end of buffer address
 * 
 * @ingroup x86_64-32bit
 */
uint64_t* create_vtable(uint64_t* page_buffer,uint64_t* lev4_page,uint64_t flag,uint64_t v_begin,uint64_t phy_begin,uint64_t phy_end){
	//align
	phy_begin = (phy_begin & ~PAGE_OFFSET_MASK);
	phy_end   = ((phy_end + PAGE_OFFSET_MASK) & ~PAGE_OFFSET_MASK);
	v_begin = (v_begin & ~PAGE_OFFSET_MASK);

	//calc v_begine index
	uint32_t lev1_index = GET_PML1_INDEX(v_begin);
	uint32_t lev2_index = GET_PML2_INDEX(v_begin);
	uint32_t lev3_index = GET_PML3_INDEX(v_begin);
	uint32_t lev4_index = GET_PML4_INDEX(v_begin);

	//calc write page counts
	uint32_t page_count = (phy_end - phy_begin) >> PML1_INDEX_SHIFT;
	uint32_t lev1_page_count = (page_count      + (PT_ENTRY_COUNT - 1)) >> PT_INDEX_BITS;
	uint32_t lev2_page_count = (lev1_page_count + (PT_ENTRY_COUNT - 1)) >> PT_INDEX_BITS;
	uint32_t lev3_page_count = (lev2_page_count + (PT_ENTRY_COUNT - 1)) >> PT_INDEX_BITS; 


	uint32_t i, j, k, l;
	//init lev3 page
	for(i = 0;i < lev3_page_count; i++){
		uint64_t* lev3_page_addr;

		//calc lev3 page addr
		if(lev4_page[i + lev4_index]){
			lev3_page_addr = (uint64_t*)(uint32_t)GET_PAGE_FRAME(lev4_page[i + lev4_index]);
		}else{
			lev3_page_addr = page_buffer;
			lev4_page[i + lev4_index] = ((uint32_t)lev3_page_addr) | PE_PRESENT;
			fill_zero(lev3_page_addr,PAGE_SIZE);
			page_buffer += PT_ENTRY_COUNT;
		}

		//init lev2 page
		for(j = lev3_index;j < (lev2_page_count + lev3_index) && j < PT_ENTRY_COUNT;j++){
			uint64_t* lev2_page_addr;

			//calc lev2 page addr
			if(lev3_page_addr[j]){
				lev2_page_addr = (uint64_t*)(uint32_t)GET_PAGE_FRAME(lev3_page_addr[j]);
			}else{
				lev2_page_addr = page_buffer;
				lev3_page_addr[j] = ((uint32_t)lev2_page_addr) | PE_PRESENT;
				fill_zero(lev2_page_addr,PAGE_SIZE);
				page_buffer += PT_ENTRY_COUNT;
			}

			//set page
			for(k = lev2_index;k < (lev1_page_count + lev2_index) && k < PT_ENTRY_COUNT;k++){
				uint64_t* lev1_page_addr;

				//calc lev1 page addr
				if(lev2_page_addr[k]){
					lev1_page_addr = (uint64_t*)(uint32_t)GET_PAGE_FRAME(lev2_page_addr[k]);
				}else{
					lev1_page_addr = page_buffer;
					lev2_page_addr[k] = ((uint32_t)lev1_page_addr) | PE_PRESENT;
					fill_zero(lev1_page_addr,PAGE_SIZE);
					page_buffer += PT_ENTRY_COUNT;
				}

				//set page
				for(l = lev1_index;l < (page_count + lev1_index) && l < PT_ENTRY_COUNT;l++){
					lev1_page_addr[l] = phy_begin | flag;
					phy_begin += 1 << PML1_INDEX_SHIFT;
				}
				page_count -= PT_ENTRY_COUNT - lev1_index;
				lev1_index = 0;
			}
			lev1_page_count -= PT_ENTRY_COUNT - lev2_index;
			lev2_index = 0;
		}

		//update
		lev2_page_count -= PT_ENTRY_COUNT - lev3_index;		
		lev3_index = 0;
	}


	return page_buffer;
}

/**
 * @brief Create a vtable object by megabyte-page
 * 
 * @param page_buffer page table buffer (need enough size)
 * @param flag page flag
 * @param v_begin virtual address begin
 * @param phy_begin physycal address begin
 * @param phy_end  physycal address end
 * @return uint64_t* end of buffer address
 * @ingroup x86_64-32bit
 */
uint64_t* create_vtable_with_pse(uint64_t* page_buffer,uint64_t* lev4_page,uint64_t flag,uint64_t v_begin,uint64_t phy_begin,uint64_t phy_end){
	//align
	phy_begin = (phy_begin & ~PAGE_OFFSET_MASK);
	phy_end   = ((phy_end + PAGE_OFFSET_MASK) & ~PAGE_OFFSET_MASK);
	v_begin = (v_begin & ~PAGE_OFFSET_MASK);

	//calc v_begine index
	uint32_t lev2_index = GET_PML2_INDEX(v_begin);
	uint32_t lev3_index = GET_PML3_INDEX(v_begin);
	uint32_t lev4_index = GET_PML4_INDEX(v_begin);

	//calc write page counts
	uint32_t big_page_count = ((phy_end - phy_begin) + PML1_INDEX_MASK) >> PML2_INDEX_SHIFT;
	uint32_t lev2_page_count = (big_page_count + (PT_ENTRY_COUNT - 1)) >> PT_INDEX_BITS;
	uint32_t lev3_page_count = (lev2_page_count + (PT_ENTRY_COUNT - 1)) >> PT_INDEX_BITS; 

	uint32_t i, j, k;
	//init lev3 page
	for(i = 0;i < lev3_page_count; i++){
		uint64_t* lev3_page_addr;

		//calc lev3 page addr
		if(lev4_page[i + lev4_index]){
			lev3_page_addr = (uint64_t*)(uint32_t)GET_PAGE_FRAME(lev4_page[i + lev4_index]);
		}else{
			lev3_page_addr = page_buffer;
			lev4_page[i + lev4_index] = ((uint32_t)lev3_page_addr) | PE_PRESENT;
			fill_zero(lev3_page_addr,PAGE_SIZE);
			page_buffer += PT_ENTRY_COUNT;
		}

		//init lev2 page
		for(j = lev3_index;j < (lev2_page_count + lev3_index) && j < PT_ENTRY_COUNT;j++){
			uint64_t* lev2_page_addr;

			//calc lev2 page addr
			if(lev3_page_addr[j]){
				lev2_page_addr = (uint64_t*)(uint32_t)GET_PAGE_FRAME(lev3_page_addr[j]);
			}else{
				lev2_page_addr = page_buffer;
				lev3_page_addr[j] = ((uint32_t)lev2_page_addr) | PE_PRESENT;
				fill_zero(lev2_page_addr,PAGE_SIZE);
				page_buffer += PT_ENTRY_COUNT;
			}

			//set page
			for(k = lev2_index;k < (big_page_count + lev2_index) && k < PT_ENTRY_COUNT;k++){
				lev2_page_addr[k] = phy_begin | flag;
				phy_begin += 1 << PML2_INDEX_SHIFT;
			}
			big_page_count -= PT_ENTRY_COUNT - lev2_index;
			lev2_index = 0;
		}

		//update
		lev2_page_count -= PT_ENTRY_COUNT - lev3_index;		
		lev3_index = 0;
	}


	return page_buffer;
}

/**
 * @brief Create a vtable object by gigabyte-page
 * 
 * @param page_buffer page table buffer (need enough size)
 * @param flag page flag
 * @param v_begin virtual address begin
 * @param phy_begin physycal address begin
 * @param phy_end  physycal address end
 * @return uint64_t* end of buffer address
 * @ingroup x86_64-32bit
 */
uint64_t* create_vtable_with_pdpe1gb(uint64_t* page_buffer,uint64_t* lev4_page,uint64_t flag,uint64_t v_begin,uint64_t phy_begin,uint64_t phy_end){
	//align
	phy_begin = (phy_begin & ~PAGE_OFFSET_MASK);
	phy_end   = ((phy_end + PAGE_OFFSET_MASK) & ~PAGE_OFFSET_MASK);
	v_begin = (v_begin & ~PAGE_OFFSET_MASK);

	//calc v_begine index
	uint32_t lev3_index = GET_PML3_INDEX(v_begin);
	uint32_t lev4_index = GET_PML4_INDEX(v_begin);

	//calc write page counts
	uint32_t big_page_count = ((phy_end - phy_begin) + PML1_INDEX_MASK + PML2_INDEX_MASK) >> PML3_INDEX_SHIFT;
	uint32_t lev3_page_count = (big_page_count + (PT_ENTRY_COUNT - 1)) >> PT_INDEX_BITS; 

	uint32_t i, j;
	//init lev3 page
	for(i = 0;i < lev3_page_count; i++){
		uint64_t* lev3_page_addr;

		//calc lev3 page addr
		if(lev4_page[i + lev4_index]){
			lev3_page_addr = (uint64_t*)(uint32_t)GET_PAGE_FRAME(lev4_page[i + lev4_index]);
		}else{
			lev3_page_addr = page_buffer;
			lev4_page[i + lev4_index] = ((uint32_t)lev3_page_addr) | PE_PRESENT;
			fill_zero(lev3_page_addr,PAGE_SIZE);
			page_buffer += PT_ENTRY_COUNT;
		}

		//init big page
		for(j = lev3_index;j < (big_page_count + lev3_index) && j < PT_ENTRY_COUNT;j++){
			lev3_page_addr[j] = phy_begin | flag;
			phy_begin += 1 << PML3_INDEX_SHIFT;
		}

		//update
		big_page_count -= PT_ENTRY_COUNT - lev3_index;		
		lev3_index = 0;
	}


	return page_buffer;
}