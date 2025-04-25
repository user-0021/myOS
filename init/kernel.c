#include <uefi/uefi.h>
#include <uefi/protocol/loaded_image.h>
#include <uefi/console/graphic.h>

typedef struct
{
	EFI_DEVICE_PATH_PROTOCOL* devPath;
	EFI_LOADED_IMAGE_PROTOCOL* bootLoaderImage;
	EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* devPathToText;
	EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL* devPathFromText;
	EFI_DEVICE_PATH_UTILITIES_PROTOCOL* devPathUtilities;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* graphicsOut;
} PROTOCOL_LIST;

typedef struct
{
	VOID* acpi20Table;
	VOID* acpiTable;
	VOID* smBios3Table;
	VOID* smBiosTable;
} TABLE_LIST;

typedef struct
{
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION Info;
} DATA_LIST;

typedef struct
{
	PROTOCOL_LIST protocols;
	TABLE_LIST tables;
	DATA_LIST data;
} BOOTLOADER_DATA;


int _kernel_start(BOOTLOADER_DATA* data){
	

	VOID *FrameBufferBase = (void*)data->protocols.graphicsOut->Mode->FrameBufferBase;
	UINT32 Width = data->data.Info.HorizontalResolution;
	UINT32 Height = data->data.Info.VerticalResolution;
	UINT32 PixelsPerScanLine = data->data.Info.PixelsPerScanLine;

	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *FB = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)FrameBufferBase;

	// 画面を真っ赤に塗る
	for (UINTN y = 0; y < Height; y++) {
		for (UINTN x = 0; x < Width; x++) {
			FB[y * PixelsPerScanLine + x].Red = 255;
			FB[y * PixelsPerScanLine + x].Green = 0;
			FB[y * PixelsPerScanLine + x].Blue = 255;
			FB[y * PixelsPerScanLine + x].Reserved = 0;
		}
	}

	while (1)
	{
		asm volatile ("hlt\n\t");
	}
	

	return 0;
}