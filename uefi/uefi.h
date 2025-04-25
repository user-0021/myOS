#pragma once
#include <uefi/uefi_defined.h>
#include <uefi/console/console.h>
#include <uefi/runtime/runtime.h>
#include <uefi/boot/boot.h>

#define EFI_SYSTEM_TABLE_SIGNATURE 0x5453595320494249
#define EFI_2_100_SYSTEM_TABLE_REVISION ((2<<16) | (100))
#define EFI_2_90_SYSTEM_TABLE_REVISION ((2<<16) | (90))
#define EFI_2_80_SYSTEM_TABLE_REVISION ((2<<16) | (80))
#define EFI_2_70_SYSTEM_TABLE_REVISION ((2<<16) | (70))
#define EFI_2_60_SYSTEM_TABLE_REVISION ((2<<16) | (60))
#define EFI_2_50_SYSTEM_TABLE_REVISION ((2<<16) | (50))
#define EFI_2_40_SYSTEM_TABLE_REVISION ((2<<16) | (40))
#define EFI_2_31_SYSTEM_TABLE_REVISION ((2<<16) | (31))
#define EFI_2_30_SYSTEM_TABLE_REVISION ((2<<16) | (30))
#define EFI_2_20_SYSTEM_TABLE_REVISION ((2<<16) | (20))
#define EFI_2_10_SYSTEM_TABLE_REVISION ((2<<16) | (10))
#define EFI_2_00_SYSTEM_TABLE_REVISION ((2<<16) | (00))
#define EFI_1_10_SYSTEM_TABLE_REVISION ((1<<16) | (10))
#define EFI_1_02_SYSTEM_TABLE_REVISION ((1<<16) | (02))
#define EFI_SYSTEM_TABLE_REVISION    EFI_2_100_SYSTEM_TABLE_REVISION
#define EFI_SPECIFICATION_VERSION    EFI_SYSTEM_TABLE_REVISION


typedef struct _EFI_CONFIGURATION_TABLE EFI_CONFIGURATION_TABLE;//EFI システム テーブルのConfigurationTableフィールドで構成される GUID/ポインター ペアのセットが含まれます 。



typedef struct _EFI_SYSTEM_TABLE{
	EFI_TABLE_HEADER                 Hdr;
	CHAR16                           *FirmwareVendor;
	UINT32                           FirmwareRevision;
	EFI_HANDLE                       ConsoleInHandle;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL   *ConIn;
	EFI_HANDLE                       ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut;
	EFI_HANDLE                       StandardErrorHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *StdErr;
	EFI_RUNTIME_SERVICES             *RuntimeServices;
	EFI_BOOT_SERVICES                *BootServices;
	UINTN                            NumberOfTableEntries;
	EFI_CONFIGURATION_TABLE          *ConfigurationTable;
} EFI_SYSTEM_TABLE;

typedef struct _EFI_CONFIGURATION_TABLE{
	EFI_GUID           VendorGuid;
	VOID               *VendorTable;
} EFI_CONFIGURATION_TABLE;