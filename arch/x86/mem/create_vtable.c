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

inline void fill_zero(void* addr,uint64_t len){
    __asm__ volatile (
        "mov %[size], %%rcx\n\t"  
        "mov %[address], %%rdi\n\t"
        "xor %%rax, %%rax\n\t"
        "rep stosl\n\t"      
        :                   
        : [address] "r"(addr), [size] "r"(len >> 2) 
        : "rcx", "rdi", "rax", "memory"
    );
}

uint64_t* create_vtable(uint64_t* page_buffer,uint64_t* lev4_page,uint64_t flag,uint64_t v_begin,uint64_t phy_begin,uint64_t phy_end){

}

uint64_t* create_vtable_with_pse(uint64_t* page_buffer,uint64_t* lev4_page,uint64_t flag,uint64_t v_begin,uint64_t phy_begin,uint64_t phy_end){

}

uint64_t* create_vtable_with_pdpe1gb(uint64_t* page_buffer,uint64_t* lev4_page,uint64_t flag,uint64_t v_begin,uint64_t phy_begin,uint64_t phy_end){
	
}