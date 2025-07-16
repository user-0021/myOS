#pragma once

#define MEMORY_USER_SPACE 			0x0000000000000000 //	ユーザプロセス空間
#define MEMORY_DIRECT_MAP_SPACE 	0xffff880000000000 //	ダイレクトマップ領域
#define MEMORY_KERNEL_CODE_SPACE 	0xffffffff80000000 //	カーネルテキストマッピング領域。物理メモリ0~512MiB