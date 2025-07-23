/**
 * @file cpuid.h
 * @author user-0021
 * @brief x86_64アーキテクチャのCPUID命令のための定数群です
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once


/**
 * @name cpuid_eax_value
 * @{
 */
/* ========================================================================= *
 * Standard CPUID EAX Input Values                      *
 * ========================================================================= */

/**
 * @ingroup x86_64
 * @brief ベンダー文字列と最大標準CPUIDレベルを取得します。
 * * @details
 * この値をEAXに設定してCPUIDを呼び出すと、以下の情報が返されます。
 * - EAX: サポートされる最大の標準CPUIDレベル。
 * - EBX, EDX, ECX: CPUベンダー文字列 (例: "GenuineIntel", "AuthenticAMD")。
 */
#define CPUID_GET_VENDOR_STRING                 0x00000000

/**
 * @ingroup x86_64
 * @brief プロセッサのバージョン情報、機能フラグを取得します。
 *
 * @details
 * この値をEAXに設定してCPUIDを呼び出すと、以下の情報が返されます。
 * - EAX: ステッピングID、モデル、ファミリーID、プロセッサタイプなど。
 * - EBX: ブランドインデックス、CLFLUSHラインサイズ、論理プロセッサ数など。
 * - ECX, EDX: SSE3, AVX, MMXなどの各種機能サポートを示すフラグ。
 */
#define CPUID_GET_PROCESSOR_INFO_AND_FEATURES   0x00000001

/**
 * @ingroup x86_64
 * @brief キャッシュとTLBのディスクリプタ情報を取得します。
 * * @details
 * この値をEAXに設定してCPUIDを呼び出すと、EAX, EBX, ECX, EDXにキャッシュやTLBの
 * 詳細な構成を示すディスクリプタが返されます。複数回呼び出す必要がある場合があります。
 */
#define CPUID_GET_CACHE_AND_TLB_INFO            0x00000002

/**
 * @ingroup x86_64
 * @brief プロセッサのシリアル番号を取得します。(現在は稀)
 * * @details
 * Pentium IIIで導入されましたが、プライバシー懸念から以降のCPUでは
 * ほとんどサポートされていません。
 */
#define CPUID_GET_PROCESSOR_SERIAL_NUMBER       0x00000003

/**
 * @ingroup x86_64
 * @brief 決定論的キャッシュパラメータを取得します。
 * * @details
 * EAX=0x02よりも構造化されたキャッシュ情報を取得します。ECXに入力として
 * キャッシュレベルを指定することで、特定のキャッシュの詳細（タイプ、サイズ、
 * ウェイ数など）が得られます。
 */
#define CPUID_GET_DETERMINISTIC_CACHE_PARAMS    0x00000004

/**
 * @ingroup x86_64
 * @brief MONITOR/MWAIT命令の機能を取得します。
 * * @details
 * スレッド同期のためのMONITOR/MWAIT命令に関する情報を取得します。
 */
#define CPUID_GET_MONITOR_MWAIT_INFO            0x00000005

/**
 * @ingroup x86_64
 * @brief サーマルおよびパワーマネジメント機能を取得します。
 * * @details
 * 温度センサーや省電力機能（SpeedStepなど）に関する情報を取得します。
 */
#define CPUID_GET_THERMAL_AND_POWER_MANAGEMENT  0x00000006

/**
 * @ingroup x86_64
 * @brief 構造化拡張機能フラグを取得します。
 * * @details
 * EAX=0x01では収まらなかった、より新しい拡張機能（AVX2, BMI1, BMI2など）の
 * サポート状況を取得します。ECX=0が主に使われます。
 * - EBX: SGX, AVX2, BMI1, BMI2などのフラグ。
 */
#define CPUID_GET_STRUCTURED_EXTENDED_FEATURES  0x00000007

/**
 * @ingroup x86_64
 * @brief パフォーマンスモニタリングに関する情報を取得します。
 *
 * @details
 * パフォーマンスカウンタのバージョン、数、ビット幅などの情報を取得します。
 */
#define CPUID_GET_PERFORMANCE_MONITORING_INFO   0x0000000A

/**
 * @ingroup x86_64
 * @brief プロセッサトポロジ（コアと論理プロセッサ）に関する情報を取得します。
 *
 * @details
 * 物理コアと論理プロセッサ（ハイパースレッディング）の階層構造を
 * 把握するために使用します。ECXに入力としてレベル（SMT/コア）を指定します。
 */
#define CPUID_GET_PROCESSOR_TOPOLOGY            0x0000000B

/* ========================================================================= *
 * Extended Function CPUID EAX Input Values                *
 * ========================================================================= */

/**
 * @ingroup x86_64
 * @brief サポートされる最大の拡張CPUIDレベルを取得します。
 * * @details
 * この値をEAXに設定してCPUIDを呼び出すと、EAXにサポートされている
 * 最大の拡張CPUIDレベル（例: 0x80000008）が返されます。
 */
#define CPUID_GET_HIGHEST_EXTENDED_FUNCTION     0x80000000

/**
 * @ingroup x86_64
 * @brief 拡張プロセッサ情報と機能フラグを取得します。
 *
 * @details
 * AMD64/Intel64(x86_64)のサポート、SYSCALL/SYSRET命令のサポートなど、
 * アーキテクチャの基本的な拡張機能に関する情報を取得します。
 */
#define CPUID_GET_EXTENDED_PROCESSOR_INFO       0x80000001

/**
 * @ingroup x86_64
 * @brief プロセッサブランド文字列を取得します (Part 1)。
 * @see CPUID_GET_BRAND_STRING_2
 * @see CPUID_GET_BRAND_STRING_3
 * * @details
 * CPUの詳細なブランド名（例: "Intel(R) Core(TM) i7-8700K CPU @ 3.70GHz"）を
 * 取得するための一連の呼び出しの最初の部分です。
 * 0x80000002から0x80000004までの3回呼び出し、EAX, EBX, ECX, EDXの値を
 * 連結することで、最大48バイトの文字列が得られます。
 */
#define CPUID_GET_BRAND_STRING_1                0x80000002

/**
 * @ingroup x86_64
 * @brief プロセッサブランド文字列を取得します (Part 2)。
 * @see CPUID_GET_BRAND_STRING_1
 */
#define CPUID_GET_BRAND_STRING_2                0x80000003

/**
 * @ingroup x86_64
 * @brief プロセッサブランド文字列を取得します (Part 3)。
 * @see CPUID_GET_BRAND_STRING_1
 */
#define CPUID_GET_BRAND_STRING_3                0x80000004

/**
 * @ingroup x86_64
 * @brief L1キャッシュとTLBの情報を取得します (AMD)。
 */
#define CPUID_GET_L1_CACHE_AND_TLB_INFO_AMD     0x80000005

/**
 * @ingroup x86_64
 * @brief 拡張L2/L3キャッシュ情報を取得します。
 *
 * @details
 * L2/L3キャッシュのサイズ、結合性（ウェイ数）などの詳細な情報を取得します。
 */
#define CPUID_GET_EXTENDED_L2_L3_CACHE_INFO     0x80000006

/**
 * @ingroup x86_64
 * @brief アドバンストパワーマネジメント情報を取得します。
 *
 * @details
 * 温度センサーや周波数スケーリングに関する拡張情報を取得します。
 */
#define CPUID_GET_ADVANCED_POWER_MANAGEMENT     0x80000007

/**
 * @ingroup x86_64
 * @brief 物理アドレスと仮想アドレスのサイズを取得します。
 *
 * @details
 * OSが利用可能な物理アドレス空間と仮想アドレス空間のビット数を取得します。
 * - EAXの下位8ビット: 物理アドレスサイズ
 * - EAXの上位8ビット: 仮想アドレスサイズ
 */
#define CPUID_GET_ADDRESS_SIZES                 0x80000008


/** @}*/


/**
 * @name processor_info_ecx
 * @{
 */
/* ========================================================================= *
 * ECX Register Feature Flags (EAX = 1)                                      *
 * ========================================================================= */

/**
 * @ingroup x86_64
 * @brief SSE3 (Streaming SIMD Extensions 3).
 * @details SSE3命令セットのサポートを示します。
 */
#define CPUID_FEAT_ECX_SSE3_SHIFT         0

/**
 * @ingroup x86_64
 * @brief PCLMULQDQ (Carry-less Multiplication).
 * @details PCLMULQDQ命令のサポートを示します。暗号化処理などで利用されます。
 */
#define CPUID_FEAT_ECX_PCLMULQDQ_SHIFT    1

/**
 * @ingroup x86_64
 * @brief DTES64 (64-bit DS Area).
 * @details 64ビットのデバッグストア(DS)エリアのサポートを示します。
 */
#define CPUID_FEAT_ECX_DTES64_SHIFT       2

/**
 * @ingroup x86_64
 * @brief MONITOR/MWAIT instructions.
 * @details スレッド待機命令MONITOR/MWAITのサポートを示します。
 */
#define CPUID_FEAT_ECX_MONITOR_SHIFT      3

/**
 * @ingroup x86_64
 * @brief DS-CPL (CPL Qualified Debug Store).
 * @details CPL(特権レベル)で制限されたデバッグストアのサポートを示します。
 */
#define CPUID_FEAT_ECX_DS_CPL_SHIFT       4

/**
 * @ingroup x86_64
 * @brief VMX (Virtual Machine Extensions).
 * @details Intelの仮想化技術(VT-x)のサポートを示します。
 */
#define CPUID_FEAT_ECX_VMX_SHIFT          5

/**
 * @ingroup x86_64
 * @brief SMX (Safer Mode Extensions).
 * @details Intelのトラステッド実行技術(TXT)のサポートを示します。
 */
#define CPUID_FEAT_ECX_SMX_SHIFT          6

/**
 * @ingroup x86_64
 * @brief EIST (Enhanced Intel SpeedStep Technology).
 * @details 拡張版Intel SpeedStep Technologyのサポートを示します。
 */
#define CPUID_FEAT_ECX_EIST_SHIFT         7

/**
 * @ingroup x86_64
 * @brief TM2 (Thermal Monitor 2).
 * @details サーマルモニター2 (TM2) のサポートを示します。
 */
#define CPUID_FEAT_ECX_TM2_SHIFT          8

/**
 * @ingroup x86_64
 * @brief SSSE3 (Supplemental Streaming SIMD Extensions 3).
 * @details SSSE3命令セットのサポートを示します。
 */
#define CPUID_FEAT_ECX_SSSE3_SHIFT        9

/**
 * @ingroup x86_64
 * @brief CNXT-ID (L1 Context ID).
 * @details L1データキャッシュのコンテキストIDのサポートを示します。
 */
#define CPUID_FEAT_ECX_CNXT_ID_SHIFT      10

/**
 * @ingroup x86_64
 * @brief SDBG (Silicon Debug).
 * @details Intelのシリコンデバッグ機能のサポートを示します。
 */
#define CPUID_FEAT_ECX_SDBG_SHIFT         11

/**
 * @ingroup x86_64
 * @brief FMA (Fused Multiply-Add).
 * @details FMA3命令セット (融合積和算) のサポートを示します。
 */
#define CPUID_FEAT_ECX_FMA_SHIFT          12

/**
 * @ingroup x86_64
 * @brief CMPXCHG16B (Compare and Exchange 16 Bytes).
 * @details 16バイトのアトミックな比較交換命令CMPXCHG16Bのサポートを示します。
 */
#define CPUID_FEAT_ECX_CX16_SHIFT         13

/**
 * @ingroup x86_64
 * @brief xTPR Update Control.
 * @details xTPR(タスク優先度レジスタ)の更新制御のサポートを示します。
 */
#define CPUID_FEAT_ECX_XTPR_SHIFT         14

/**
 * @ingroup x86_64
 * @brief PDCM (Perfmon and Debug Capability).
 * @details パフォーマンスモニタリングとデバッグ能力のサポートを示します。
 */
#define CPUID_FEAT_ECX_PDCM_SHIFT         15

// Bit 16 is Reserved.

/**
 * @ingroup x86_64
 * @brief PCID (Process-context identifiers).
 * @details TLBを効率化するPCIDのサポートを示します。
 */
#define CPUID_FEAT_ECX_PCID_SHIFT         17

/**
 * @ingroup x86_64
 * @brief DCA (Direct Cache Access).
 * @details デバイスからCPUキャッシュへの直接データ配置(DCA)のサポートを示します。
 */
#define CPUID_FEAT_ECX_DCA_SHIFT          18

/**
 * @ingroup x86_64
 * @brief SSE4.1 (Streaming SIMD Extensions 4.1).
 * @details SSE4.1命令セットのサポートを示します。
 */
#define CPUID_FEAT_ECX_SSE4_1_SHIFT       19

/**
 * @ingroup x86_64
 * @brief SSE4.2 (Streaming SIMD Extensions 4.2).
 * @details SSE4.2命令セットのサポートを示します。
 */
#define CPUID_FEAT_ECX_SSE4_2_SHIFT       20

/**
 * @ingroup x86_64
 * @brief x2APIC (Extended xAPIC Support).
 * @details 拡張APICであるx2APICのサポートを示します。
 */
#define CPUID_FEAT_ECX_X2APIC_SHIFT       21

/**
 * @ingroup x86_64
 * @brief MOVBE (Move Big-Endian Instruction).
 * @details ビッグエンディアンとリトルエンディアンを変換するMOVBE命令のサポートを示します。
 */
#define CPUID_FEAT_ECX_MOVBE_SHIFT        22

/**
 * @ingroup x86_64
 * @brief POPCNT (Pop Count Instruction).
 * @details データ内の1のビット数を数えるPOPCNT命令のサポートを示します。
 */
#define CPUID_FEAT_ECX_POPCNT_SHIFT       23

/**
 * @ingroup x86_64
 * @brief TSC-Deadline Timer.
 * @details APICがTSCデッドラインタイマーをサポートしていることを示します。
 */
#define CPUID_FEAT_ECX_TSC_DEADLINE_SHIFT 24

/**
 * @ingroup x86_64
 * @brief AES-NI (AES New Instructions).
 * @details AES暗号化/復号を高速化するAES-NI命令セットのサポートを示します。
 */
#define CPUID_FEAT_ECX_AESNI_SHIFT        25

/**
 * @ingroup x86_64
 * @brief XSAVE/XRSTOR instructions.
 * @details 拡張プロセッサ状態を保存/復元するXSAVE命令群のサポートを示します。
 */
#define CPUID_FEAT_ECX_XSAVE_SHIFT        26

/**
 * @ingroup x86_64
 * @brief OSXSAVE (OS has enabled XSAVE).
 * @details OSがXSAVEを有効にするためにCR4.OSXSAVEビットを設定したかを示します。
 */
#define CPUID_FEAT_ECX_OSXSAVE_SHIFT      27

/**
 * @ingroup x86_64
 * @brief AVX (Advanced Vector Extensions).
 * @details AVX命令セットのサポートを示します。
 */
#define CPUID_FEAT_ECX_AVX_SHIFT          28

/**
 * @ingroup x86_64
 * @brief F16C (16-bit Floating-Point conversion).
 * @details 16ビット半精度浮動小数点数の変換命令のサポートを示します。
 */
#define CPUID_FEAT_ECX_F16C_SHIFT         29

/**
 * @ingroup x86_64
 * @brief RDRAND (Read Random Number).
 * @details ハードウェア乱数生成器から乱数を読み取るRDRAND命令のサポートを示します。
 */
#define CPUID_FEAT_ECX_RDRAND_SHIFT       30

/**
 * @ingroup x86_64
 * @brief Hypervisor.
 * @details 仮想マシンモニタ(ハイパーバイザ)上で実行されているかを示します。
 */
#define CPUID_FEAT_ECX_HYPERVISOR_SHIFT   31

/** @}*/


/**
 * @name processor_info_edx
 * @{
 */

/* ========================================================================= *
 * EDX Register Feature Flags (EAX = 1)                                      *
 * ========================================================================= */

/**
 * @ingroup x86_64
 * @brief FPU (Floating-Point Unit on-Chip).
 * @details オンチップx87 FPUの存在を示します。
 */
#define CPUID_FEAT_EDX_FPU_SHIFT          0

/**
 * @ingroup x86_64
 * @brief VME (Virtual Mode Extension).
 * @details 仮想8086モード拡張のサポートを示します。
 */
#define CPUID_FEAT_EDX_VME_SHIFT          1

/**
 * @ingroup x86_64
 * @brief DE (Debugging Extension).
 * @details I/Oブレークポイントを含むデバッグ拡張のサポートを示します。
 */
#define CPUID_FEAT_EDX_DE_SHIFT           2

/**
 * @ingroup x86_64
 * @brief PSE (Page Size Extension).
 * @details 4MBページのサポートを示します。
 */
#define CPUID_FEAT_EDX_PSE_SHIFT          3

/**
 * @ingroup x86_64
 * @brief TSC (Time Stamp Counter).
 * @details RDTSC命令とTSCレジスタのサポートを示します。
 */
#define CPUID_FEAT_EDX_TSC_SHIFT          4

/**
 * @ingroup x86_64
 * @brief MSR (Model Specific Registers).
 * @details RDMSRおよびWRMSR命令のサポートを示します。
 */
#define CPUID_FEAT_EDX_MSR_SHIFT          5

/**
 * @ingroup x86_64
 * @brief PAE (Physical Address Extension).
 * @details 36ビット物理アドレス拡張のサポートを示します。
 */
#define CPUID_FEAT_EDX_PAE_SHIFT          6

/**
 * @ingroup x86_64
 * @brief MCE (Machine Check Exception).
 * @details マシンチェック例外(#MC) のサポートを示します。
 */
#define CPUID_FEAT_EDX_MCE_SHIFT          7

/**
 * @ingroup x86_64
 * @brief CX8 (CMPXCHG8B instruction).
 * @details 8バイトのアトミックな比較交換命令CMPXCHG8Bのサポートを示します。
 */
#define CPUID_FEAT_EDX_CX8_SHIFT          8

/**
 * @ingroup x86_64
 * @brief APIC (On-chip APIC Hardware).
 * @details オンチップAPICのハードウェアサポートを示します。
 */
#define CPUID_FEAT_EDX_APIC_SHIFT         9

// Bit 10 is Reserved.

/**
 * @ingroup x86_64
 * @brief SEP (SYSENTER/SYSEXIT instructions).
 * @details 高速なシステムコール命令であるSYSENTER/SYSEXITのサポートを示します。
 */
#define CPUID_FEAT_EDX_SEP_SHIFT          11

/**
 * @ingroup x86_64
 * @brief MTRR (Memory Type Range Registers).
 * @details メモリタイプの範囲を指定するレジスタのサポートを示します。
 */
#define CPUID_FEAT_EDX_MTRR_SHIFT         12

/**
 * @ingroup x86_64
 * @brief PGE (Page Global Enable).
 * @details CR4レジスタのPGEビットによるグローバルページのサポートを示します。
 */
#define CPUID_FEAT_EDX_PGE_SHIFT          13

/**
 * @ingroup x86_64
 * @brief MCA (Machine Check Architecture).
 * @details マシンチェック例外の拡張機能であるMCAのサポートを示します。
 */
#define CPUID_FEAT_EDX_MCA_SHIFT          14

/**
 * @ingroup x86_64
 * @brief CMOV (Conditional Move Instruction).
 * @details CMOV命令およびFCMOV命令のサポートを示します。
 */
#define CPUID_FEAT_EDX_CMOV_SHIFT         15

/**
 * @ingroup x86_64
 * @brief PAT (Page Attribute Table).
 * @details ページ属性テーブルのサポートを示します。
 */
#define CPUID_FEAT_EDX_PAT_SHIFT          16

/**
 * @ingroup x86_64
 * @brief PSE-36 (36-bit Page Size Extension).
 * @details 36ビットモードでのページサイズ拡張のサポートを示します。
 */
#define CPUID_FEAT_EDX_PSE36_SHIFT        17

/**
 * @ingroup x86_64
 * @brief PSN (Processor Serial Number).
 * @details プロセッサシリアルナンバーのサポートを示します (現在は無効)。
 */
#define CPUID_FEAT_EDX_PSN_SHIFT          18

/**
 * @ingroup x86_64
 * @brief CLFSH (CLFLUSH instruction).
 * @details CLFLUSH命令のサポートを示します。
 */
#define CPUID_FEAT_EDX_CLFSH_SHIFT        19

// Bit 20 is Reserved.

/**
 * @ingroup x86_64
 * @brief DS (Debug Store).
 * @details デバッグトレースストア(DTS)のサポートを示します。
 */
#define CPUID_FEAT_EDX_DS_SHIFT           21

/**
 * @ingroup x86_64
 * @brief ACPI (Thermal Monitor and Software Controlled Clock).
 * @details ACPIによる熱監視とクロック制御のサポートを示します。
 */
#define CPUID_FEAT_EDX_ACPI_SHIFT         22

/**
 * @ingroup x86_64
 * @brief MMX (MMX technology).
 * @details MMX命令セットのサポートを示します。
 */
#define CPUID_FEAT_EDX_MMX_SHIFT          23

/**
 * @ingroup x86_64
 * @brief FXSR (FXSAVE/FXRSTOR instructions).
 * @details 高速なFPU/MMX/SSEコンテキスト保存・復元命令のサポートを示します。
 */
#define CPUID_FEAT_EDX_FXSR_SHIFT         24

/**
 * @ingroup x86_64
 * @brief SSE (Streaming SIMD Extensions).
 * @details SSE命令セットのサポートを示します。
 */
#define CPUID_FEAT_EDX_SSE_SHIFT          25

/**
 * @ingroup x86_64
 * @brief SSE2 (Streaming SIMD Extensions 2).
 * @details SSE2命令セットのサポートを示します。
 */
#define CPUID_FEAT_EDX_SSE2_SHIFT         26

/**
 * @ingroup x86_64
 * @brief SS (Self-Snoop).
 * @details CPUキャッシュのセルフスヌープ機能のサポートを示します。
 */
#define CPUID_FEAT_EDX_SS_SHIFT           27

/**
 * @ingroup x86_64
 * @brief HTT (Hyper-Threading Technology).
 * @details ハイパースレッディング・テクノロジーのサポートを示します。
 */
#define CPUID_FEAT_EDX_HTT_SHIFT          28

/**
 * @ingroup x86_64
 * @brief TM (Thermal Monitor).
 * @details 熱監視機能(TM1)のサポートを示します。
 */
#define CPUID_FEAT_EDX_TM_SHIFT           29

/**
 * @ingroup x86_64
 * @brief IA64 (IA64 processor emulating x86).
 * @details IA64プロセッサがx86をエミュレートしていることを示します (現在は意味をなしません)。
 */
#define CPUID_FEAT_EDX_IA64_SHIFT         30

/**
 * @ingroup x86_64
 * @brief PBE (Pending Break Enable).
 * @details 割り込み遅延によるブレーク機能のサポートを示します。
 */
#define CPUID_FEAT_EDX_PBE_SHIFT          31

/** @}*/


/**
 * @name extended_processor_info_ecx
 * @{
 */
/* ========================================================================= *
 * ECX Register Flags (EAX = 0x80000001)                                     *
 * ========================================================================= */

/**
 * @ingroup x86_64
 * @brief LAHF/SAHF in 64-bit mode.
 * @details 64ビットモードでのLAHF/SAHF命令のサポートを示します。
 */
#define CPUID_EX_ECX_LAHF_SAHF_SHIFT    0

/**
 * @ingroup x86_64
 * @brief CmpLegacy (Core Multi-Processing legacy mode).
 * @details ハイパースレッディングが有効な場合に、コア数を報告するかどうかを示します (AMD)。
 */
#define CPUID_EX_ECX_CMP_LEGACY_SHIFT   1

/**
 * @ingroup x86_64
 * @brief SVM (Secure Virtual Machine).
 * @details AMDの仮想化技術(AMD-V)のサポートを示します。
 */
#define CPUID_EX_ECX_SVM_SHIFT          2

/**
 * @ingroup x86_64
 * @brief AltMovCr8 (Alternate MOV CR8).
 * @details LOCK MOV CR8命令のサポートを示します。
 */
#define CPUID_EX_ECX_ALT_MOV_CR8_SHIFT  4

/**
 * @ingroup x86_64
 * @brief ABM (Advanced Bit Manipulation).
 * @details LZCNTおよびPOPCNT命令のサポートを示します。
 */
#define CPUID_EX_ECX_ABM_SHIFT          5

/**
 * @ingroup x86_64
 * @brief SSE4A (SSE4A extensions).
 * @details AMDのSSE4A拡張命令セットのサポートを示します。
 */
#define CPUID_EX_ECX_SSE4A_SHIFT        6

/**
 * @ingroup x86_64
 * @brief MisAlignSse (Misaligned SSE mode).
 * @details アライメントされていないメモリアクセスに対するSSE命令のサポートを示します。
 */
#define CPUID_EX_ECX_MISALIGN_SSE_SHIFT 7

/**
 * @ingroup x86_64
 * @brief 3DNowPrefetch (3DNow! prefetch).
 * @details PREFETCH/PREFETCHW命令のサポートを示します。
 */
#define CPUID_EX_ECX_3DNOW_PREFETCH_SHIFT 8

/**
 * @ingroup x86_64
 * @brief OSVW (OS-visible workaround).
 * @details OSが対応すべきハードウェアの問題を示唆します。
 */
#define CPUID_EX_ECX_OSVW_SHIFT         9

/**
 * @ingroup x86_64
 * @brief IBS (Instruction Based Sampling).
 * @details 命令ベースのサンプリング機能のサポートを示します (AMD)。
 */
#define CPUID_EX_ECX_IBS_SHIFT          10

/**
 * @ingroup x86_64
 * @brief XOP (eXtended Operations).
 * @details AMDのXOP拡張命令セットのサポートを示します。
 */
#define CPUID_EX_ECX_XOP_SHIFT          11

/**
 * @ingroup x86_64
 * @brief SKINIT (SKINIT and STGI are supported).
 * @details AMDのセキュアブート関連命令のサポートを示します。
 */
#define CPUID_EX_ECX_SKINIT_SHIFT       12

/**
 * @ingroup x86_64
 * @brief TBM (Trailing Bit Manipulation).
 * @details AMDのTBM拡張命令セットのサポートを示します。
 */
#define CPUID_EX_ECX_TBM_SHIFT          21

/**
 * @ingroup x86_64
 * @brief TopologyExtensions (Topology extensions).
 * @details 拡張APIC IDやプロセッサトポロジ情報のサポートを示します。
 */
#define CPUID_EX_ECX_TOPO_EXT_SHIFT     22


/** @}*/


/**
 * @name extended_processor_info_edx
 * @{
 */
/* ========================================================================= *
 * EDX Register Flags (EAX = 0x80000001)                                     *
 * ========================================================================= */

/**
 * @ingroup x86_64
 * @brief FPU (Floating-Point Unit on-Chip).
 * @details オンチップx87 FPUの存在を示します。
 */
#define CPUID_EX_EDX_FPU_SHIFT      0

/**
 * @ingroup x86_64
 * @brief VME (Virtual Mode Extension).
 * @details 仮想8086モード拡張のサポートを示します。
 */
#define CPUID_EX_EDX_VME_SHIFT      1

/**
 * @ingroup x86_64
 * @brief DE (Debugging Extension).
 * @details I/Oブレークポイントを含むデバッグ拡張のサポートを示します。
 */
#define CPUID_EX_EDX_DE_SHIFT       2

/**
 * @ingroup x86_64
 * @brief PSE (Page Size Extension).
 * @details 4MBページのサポートを示します。
 */
#define CPUID_EX_EDX_PSE_SHIFT      3

/**
 * @ingroup x86_64
 * @brief TSC (Time Stamp Counter).
 * @details RDTSC命令のサポートを示します。
 */
#define CPUID_EX_EDX_TSC_SHIFT      4

/**
 * @ingroup x86_64
 * @brief MSR (Model Specific Registers).
 * @details RDMSRおよびWRMSR命令のサポートを示します。
 */
#define CPUID_EX_EDX_MSR_SHIFT      5

/**
 * @ingroup x86_64
 * @brief PAE (Physical Address Extension).
 * @details 36ビット物理アドレス拡張のサポートを示します。
 */
#define CPUID_EX_EDX_PAE_SHIFT      6

/**
 * @ingroup x86_64
 * @brief MCE (Machine Check Exception).
 * @details マシンチェック例外(#MC) のサポートを示します。
 */
#define CPUID_EX_EDX_MCE_SHIFT      7

/**
 * @ingroup x86_64
 * @brief CMPXCHG8B (Compare and Exchange 8 Bytes).
 * @details CMPXCHG8B命令のサポートを示します。
 */
#define CPUID_EX_EDX_CX8_SHIFT      8

/**
 * @ingroup x86_64
 * @brief APIC (On-chip APIC Hardware support).
 * @details オンチップAPICのハードウェアサポートを示します。
 */
#define CPUID_EX_EDX_APIC_SHIFT     9

// ビット10は予約済み (Reserved)

/**
 * @ingroup x86_64
 * @brief SYSCALL/SYSRET instructions.
 * @details 高速なシステムコール命令であるSYSCALLとSYSRETのサポートを示します (AMD64/Intel64のLong Modeで利用)。
 */
#define CPUID_EX_EDX_SYSCALL_SHIFT  11

/**
 * @ingroup x86_64
 * @brief MTRR (Memory Type Range Registers).
 * @details メモリタイプの範囲を指定するレジスタのサポートを示します。
 */
#define CPUID_EX_EDX_MTRR_SHIFT     12

/**
 * @ingroup x86_64
 * @brief PGE (Page Global Enable).
 * @details CR4レジスタのPGEビットによるグローバルページのサポートを示します。
 */
#define CPUID_EX_EDX_PGE_SHIFT      13

/**
 * @ingroup x86_64
 * @brief MCA (Machine Check Architecture).
 * @details マシンチェック例外の拡張機能であるMCAのサポートを示します。
 */
#define CPUID_EX_EDX_MCA_SHIFT      14

/**
 * @ingroup x86_64
 * @brief CMOV (Conditional Move Instruction).
 * @details CMOV命令のサポートを示します。
 */
#define CPUID_EX_EDX_CMOV_SHIFT     15

/**
 * @ingroup x86_64
 * @brief PAT (Page Attribute Table).
 * @details ページ属性テーブルのサポートを示します。
 */
#define CPUID_EX_EDX_PAT_SHIFT      16

/**
 * @ingroup x86_64
 * @brief PSE-36 (36-bit Page Size Extension).
 * @details 36ビットモードでのページサイズ拡張のサポートを示します。
 */
#define CPUID_EX_EDX_PSE36_SHIFT    17

// ビット18は予約済み (Reserved)

/**
 * @ingroup x86_64
 * @brief MP (Multiprocessor Capable).
 * @details マルチプロセッサ構成が可能であることを示します (現在はあまり使われない)。
 */
#define CPUID_EX_EDX_MP_SHIFT       19

/**
 * @ingroup x86_64
 * @brief NX (No-Execute bit).
 * @details データ実行防止機能(NXビット)のサポートを示します。
 */
#define CPUID_EX_EDX_NX_SHIFT       20

// ビット21は予約済み (Reserved)

/**
 * @ingroup x86_64
 * @brief MMXEXT (AMD extensions to MMX).
 * @details MMXに対するAMDの拡張命令セットのサポートを示します。
 */
#define CPUID_EX_EDX_MMXEXT_SHIFT   22

/**
 * @ingroup x86_64
 * @brief MMX (MMX technology).
 * @details MMX命令のサポートを示します。(CPUID EAX=1のEDX[23]と同じ)
 */
#define CPUID_EX_EDX_MMX_SHIFT      23

/**
 * @ingroup x86_64
 * @brief FXSR (FXSAVE/FXRSTOR instructions).
 * @details 高速なFPU/MMX/SSEコンテキスト保存・復元命令のサポートを示します。
 */
#define CPUID_EX_EDX_FXSR_SHIFT     24

/**
 * @ingroup x86_64
 * @brief FFXSR (FXSAVE/FXRSTOR optimizations).
 * @details FXSR命令の最適化版のサポートを示します。
 */
#define CPUID_EX_EDX_FFXSR_SHIFT    25

/**
 * @ingroup x86_64
 * @brief Page1GB (1-GB Pages).
 * @details 1GBページのサポートを示します。
 */
#define CPUID_EX_EDX_PAGE1GB_SHIFT  26

/**
 * @ingroup x86_64
 * @brief RDTSCP (Read Time-Stamp Counter and Processor ID).
 * @details RDTSCP命令のサポートを示します。順序保証付きのTSC読み出しが可能です。
 */
#define CPUID_EX_EDX_RDTSCP_SHIFT   27

// ビット28は予約済み (Reserved)

/**
 * @ingroup x86_64
 * @brief LM (Long Mode).
 * @details AMD64またはIntel 64 (x86-64) の64ビットモード(Long Mode)のサポートを示します。
 */
#define CPUID_EX_EDX_LM_SHIFT       29

/**
 * @ingroup x86_64
 * @brief 3DNow! (3DNow! extensions).
 * @details 3DNow!拡張命令セットのサポートを示します。
 */
#define CPUID_EX_EDX_3DNOWEXT_SHIFT 30

/**
 * @ingroup x86_64
 * @brief 3DNow! (3DNow! technology).
 * @details 3DNow!命令セットのサポートを示します。
 */
#define CPUID_EX_EDX_3DNOW_SHIFT    31


/** @}*/