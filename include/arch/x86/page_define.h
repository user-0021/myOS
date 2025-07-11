#pragma once


// --- ページングレベルに関する定義 ---
// x86-64は4レベルページング（またはCR4.LA57有効で5レベル）をサポート
// 通常は4レベルページングを前提とします。
// 各レベルのエントリは512個 (9ビットインデックス)

#define PT_ENTRY_COUNT 512       // 各ページテーブルレベルのエントリ数
#define PT_INDEX_BITS  9         // 各レベルのページテーブルインデックスに必要なビット数

// 各レベルのオフセットマスクとシフト量
// 仮想アドレス: 47-39 (PML4E), 38-30 (PDPTE), 29-21 (PDE), 20-12 (PTE), 11-0 (Offset)

// PML4 (Page Map Level 4) - 47ビット目から39ビット目
#define PML4_INDEX_SHIFT   39
#define PML4_INDEX_MASK    (0x1FFULL << PML4_INDEX_SHIFT) // 9ビットのインデックス

// PDPT (Page Directory Pointer Table) - 38ビット目から30ビット目
#define PDPT_INDEX_SHIFT   30
#define PDPT_INDEX_MASK    (0x1FFULL << PDPT_INDEX_SHIFT) // 9ビットのインデックス

// PD (Page Directory) - 29ビット目から21ビット目
#define PD_INDEX_SHIFT     21
#define PD_INDEX_MASK      (0x1FFULL << PD_INDEX_SHIFT) // 9ビットのインデックス

// PT (Page Table) - 20ビット目から12ビット目
#define PT_INDEX_SHIFT     12
#define PT_INDEX_MASK      (0x1FFULL << PT_INDEX_SHIFT) // 9ビットのインデックス

// ページオフセット - 11ビット目から0ビット目
#define PAGE_OFFSET_MASK   0xFFFULL      // 12ビットのオフセット (4KBページ)

#define PAGE_SIZE          4096ULL     // 4KBページサイズ
#define PAGE_SIZE_SHIFT    12          // ページサイズを表すビット数 (log2(4096))
#define PAGE_ALIGN(addr)   ((addr) & ~(PAGE_SIZE - 1)) // アドレスをページ境界にアライン

// --- ページエントリフラグに関する定義 ---
// 各ページテーブルエントリの下位12ビットはフラグ

// 共通フラグ (PML4E, PDPTE, PDE, PTEに共通して存在する可能性のあるフラグ)
#define PE_PRESENT         (1ULL << 0)   // P (Present): ページまたはページテーブルが存在する
#define PE_RW              (1ULL << 1)   // R/W (Read/Write): 0=読み取り専用, 1=読み書き可能
#define PE_US              (1ULL << 2)   // U/S (User/Supervisor): 0=スーパーバイザーのみ, 1=ユーザーアクセス可能
#define PE_PWT             (1ULL << 3)   // PWT (Page Write Through): 0=ライトバック, 1=ライトスルー
#define PE_PCD             (1ULL << 4)   // PCD (Page Cache Disable): 0=キャッシュ可能, 1=キャッシュ無効
#define PE_ACCESSED        (1ULL << 5)   // A (Accessed): ページまたはページテーブルにアクセスがあった
#define PE_DIRTY           (1ULL << 6)   // D (Dirty): ページが書き込まれた (PTEレベルのみ有効)
#define PE_PAT             (1ULL << 7)   // PAT (Page Attribute Table): CPUモードによって異なる意味を持つ
                                       // (通常はPTEのみだが、PDPTE, PDEで1GB/2MBページにも適用)
#define PE_GLOBAL          (1ULL << 8)   // G (Global): TLBでグローバルエントリとして扱う (CR4.PGEがセットされている場合)
#define PE_NX              (1ULL << 63)  // XD (Execute Disable): 実行不可 (上位ビット)

// large pages (2MB / 1GB) 固有のフラグ
// PDPTEやPDEでPE_PS (Page Size)フラグがセットされている場合、それらはページテーブルではなく直接ページを指す
#define PE_PS              (1ULL << 7)   // PS (Page Size): 0=次のレベルのテーブル, 1=ラージページ

// アドレス部分のマスク (フラグを除いた上位ビット)
#define PAGE_FRAME_MASK    0xFFFFFFFFFFFFF000ULL // 物理アドレスフレーム部分 (上位52ビット中12ビットオフセットを引いた部分)

// ページエントリの物理アドレスを取得するマクロ
#define GET_PAGE_FRAME(entry) ((entry) & PAGE_FRAME_MASK)

// ページエントリにフラグを設定するマクロ
#define SET_FLAG(entry, flag)    ((entry) | (flag))
#define CLEAR_FLAG(entry, flag)  ((entry) & ~(flag))
#define HAS_FLAG(entry, flag)    (((entry) & (flag)) == (flag))


#ifndef __ASSEMBLER__
#include <stdint.h>

//各種GET
#define GET_PML4_INDEX(addr) (((uint64_t)(addr) & PML4_INDEX_MASK) >> PML4_INDEX_SHIFT)
#define GET_PDPT_INDEX(addr) (((uint64_t)(addr) & PDPT_INDEX_MASK) >> PDPT_INDEX_SHIFT)
#define GET_PD_INDEX(addr) (((uint64_t)(addr) & PD_INDEX_MASK) >> PD_INDEX_SHIFT)
#define GET_PT_INDEX(addr) (((uint64_t)(addr) & PT_INDEX_MASK) >> PT_INDEX_SHIFT)
#define GET_PAGE_OFFSET(addr) ((uint64_t)(addr) & PAGE_OFFSET_MASK)
#endif