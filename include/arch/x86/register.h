/**
 * @file register.h
 * @author user-0021
 * @brief x86_64の各レジスタのための定数を生成します
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once




/**
 * @name CR0 Flags
 * @{
 */
 /**
  * @ingroup x86_64
  */
#define CR0_PE  (1UL << 0)  /**< 保護モード有効 (Protection Enable) */
#define CR0_MP  (1UL << 1)  /**< コプロセッサ監視 (Monitor Coprocessor) */
#define CR0_EM  (1UL << 2)  /**< FPUエミュレーション (Emulation) */
#define CR0_TS  (1UL << 3)  /**< タスクスイッチ発生 (Task Switched) */
#define CR0_ET  (1UL << 4)  /**< 拡張タイプ (Extension Type, 予約済み) */
#define CR0_NE  (1UL << 5)  /**< 数値エラー有効 (Numeric Error) */
#define CR0_WP  (1UL << 16) /**< 書き込み保護 (Write Protect) */
#define CR0_AM  (1UL << 18) /**< アライメントマスク (Alignment Mask) */
#define CR0_NW  (1UL << 29) /**< ライトスルー無効 (Not Write-through) */
#define CR0_CD  (1UL << 30) /**< キャッシュ無効 (Cache Disable) */
#define CR0_PG  (1UL << 31) /**< ページング有効 (Paging) */
/** @} */

/**
 * @name CR4 Flags
 * @{
 */
 /**
  * @ingroup x86_64
  */
#define CR4_VME         (1UL << 0)  /**< 仮想8086モード拡張 (Virtual-8086 Mode Extensions) */
#define CR4_PVI         (1UL << 1)  /**< 保護モード仮想割り込み (Protected-Mode Virtual Interrupts) */
#define CR4_TSD         (1UL << 2)  /**< タイムスタンプ無効 (Time Stamp Disable) */
#define CR4_DE          (1UL << 3)  /**< デバッグ拡張 (Debugging Extensions) */
#define CR4_PSE         (1UL << 4)  /**< ページサイズ拡張 (Page Size Extension, 4MBページ) */
#define CR4_PAE         (1UL << 5)  /**< 物理アドレス拡張 (Physical Address Extension) */
#define CR4_MCE         (1UL << 6)  /**< マシンチェック有効 (Machine-Check Enable) */
#define CR4_PGE         (1UL << 7)  /**< ページグローバル有効 (Page Global Enable) */
#define CR4_PCE         (1UL << 8)  /**< パフォーマンスモニタカウンタ有効 (Performance-Monitoring Counter Enable) */
#define CR4_OSFXSR      (1UL << 9)  /**< OSによるFXSAVE/FXRSTOR命令のサポート (OS Support for FXSAVE/FXRSTOR) */
#define CR4_OSXMMEXCPT  (1UL << 10) /**< OSによるSIMD浮動小数点例外のサポート (OS Support for Unmasked SIMD Exceptions) */
#define CR4_UMIP        (1UL << 11) /**< ユーザーモード命令禁止 (User-Mode Instruction Prevention) */
#define CR4_LA57        (1UL << 12) /**< 57ビットリニアアドレス有効 (57-bit Linear Addresses) */
#define CR4_VMXE        (1UL << 13) /**< VMX有効化 (VMX-Enable Bit) */
#define CR4_SMXE        (1UL << 14) /**< SMX有効化 (SMX-Enable Bit) */
#define CR4_FSGSBASE    (1UL << 16) /**< RDFSBASE/RDGSBASE/WRFSBASE/WRGSBASE命令の有効化 */
#define CR4_PCIDE       (1UL << 17) /**< PCID有効化 (PCID-Enable Bit) */
#define CR4_OSXSAVE     (1UL << 18) /**< OSXSAVE有効化 (XSAVE and Processor Extended States-Enable Bit) */
#define CR4_SMEP        (1UL << 20) /**< スーパーバイザモード実行保護 (Supervisor Mode Execution Protection) */
#define CR4_SMAP        (1UL << 21) /**< スーパーバイザモードアクセス保護 (Supervisor Mode Access Prevention) */
#define CR4_PKE         (1UL << 22) /**< 保護キー有効化 (Protection Key Enable) */
/** @} */

/** 
 * @name MSR index
 * @{
 */
 /**
  * @ingroup x86_64
  */
//==============================================================================
// Intel/AMD共通 MSR 定数定義
//==============================================================================

/// @brief EFER (Extended Feature Enable Register)
///
/// x86-64モードの有効化、System Call/Sysret命令の有効化、NX (No-Execute) ビットの有効化など、
/// 拡張機能の有効/無効を制御する重要なMSR。
#define MSR_EFER 0xC0000080

/// @brief STAR (System Call Target Address Register)
///
/// `syscall`命令がジャンプするセグメントセレクタのベースアドレスを定義します。
/// これは、ユーザーモードからカーネルモードへのエントリポイントを設定するために使用されます。
#define MSR_STAR 0xC0000081

/// @brief LSTAR (Long Mode System Call Target Address Register)
///
/// `syscall`命令がジャンプする64ビットのRIP (Instruction Pointer) を定義します。
/// 64ビットのカーネルエントリポイントを指定するために使用されます。
#define MSR_LSTAR 0xC0000082

/// @brief CSTAR (Compatibility Mode System Call Target Address Register)
///
/// `syscall`命令が互換モード（32ビット）で実行された場合のRIPを定義します。
#define MSR_CSTAR 0xC0000083

/// @brief SFMASK (System Call Flag Mask Register)
///
/// `syscall`命令で、ユーザーからカーネルへ移行する際にEFLAGSレジスタのどのビットをクリアするかを制御します。
#define MSR_SFMASK 0xC0000084

/// @brief Kernel GS Base (Kernel GS Base Address Register)
///
/// GSレジスタのベースアドレスをカーネルが設定するために使用します。
/// `swapgs`命令でユーザーGSと切り替えるためのアドレスを保持します。
#define MSR_KERNEL_GS_BASE 0xC0000102

/// @brief TSX_CTRL (Transactional Synchronization Extensions Control)
///
/// TSX (Transactional Synchronization Extensions) の有効/無効や制御を行います。
/// ただし、プロセッサによっては存在しない可能性があります。
#define MSR_TSX_CTRL 0x00000122
/**
 * @}
 */

/**
 * @name MSR EFER Flag
 * @{
 */
/**
 * @ingroup x86_64
 */

/// @brief SCE: System Call Extensions Enable
///
/// `syscall` および `sysret` 命令を有効にします。
/// このビットが`1`の場合、AMD64命令セットの`syscall`/`sysret`が利用可能になります。
#define MSR_EFER_SCE (1ULL << 0)

/// @brief LME: Long Mode Enable
///
/// プロセッサをロングモード（64ビットモード）に移行させるためのイネーブルビットです。
/// このビットが`1`の場合、ページングが有効であれば、ロングモードが有効になります。
#define MSR_EFER_LME (1ULL << 8)

/// @brief LMA: Long Mode Active
///
/// プロセッサが現在ロングモードで動作していることを示すステータスビットです。
/// このビットは通常、プロセッサによって設定されるため、ソフトウェアが直接書き込むことはありません。
#define MSR_EFER_LMA (1ULL << 10)

/// @brief NXE: No-Execute Enable
///
/// NX (No-Execute) ビットを有効にします。これにより、ページテーブルエントリの
/// NXビットが有効になり、データページからのコード実行を防止できます。
#define MSR_EFER_NXE (1ULL << 11)

/// @brief SVME: Secure Virtual Machine Enable (AMD-V)
///
/// AMDのハードウェア仮想化機能（AMD-V）を有効にします。
/// このビットが`1`の場合、SVM (Secure Virtual Machine) の機能が有効になります。
#define MSR_EFER_SVME (1ULL << 12)

/// @brief VMXE: Virtual Machine Extensions Enable (Intel VT-x)
///
/// Intelのハードウェア仮想化機能（VT-x）を有効にします。
/// このビットが`1`の場合、VMX (Virtual Machine Extensions) の機能が有効になります。
/// @note このフラグはIntelプロセッサのみに存在します。
#define MSR_EFER_VMXE (1ULL << 13)

/**
 * @}
 */