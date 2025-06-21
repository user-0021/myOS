
int _kernel_start(){
	int a = 2;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	a++;
	char* as = "HEllop aijdas";
	
	return as[a];
}

//#include <uefi/uefi.h>
//#include <uefi/protocol/loaded_image.h>
//#include <uefi/console/graphic.h>

//typedef struct
//{
//	EFI_DEVICE_PATH_PROTOCOL* devPath;
//	EFI_LOADED_IMAGE_PROTOCOL* bootLoaderImage;
//	EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* devPathToText;
//	EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL* devPathFromText;
//	EFI_DEVICE_PATH_UTILITIES_PROTOCOL* devPathUtilities;
//	EFI_GRAPHICS_OUTPUT_PROTOCOL* graphicsOut;
//} PROTOCOL_LIST;

//typedef struct
//{
//	VOID* acpi20Table;
//	VOID* acpiTable;
//	VOID* smBios3Table;
//	VOID* smBiosTable;
//} TABLE_LIST;

//typedef struct
//{
//	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION gInfo;
//	EFI_PHYSICAL_ADDRESS screenBuffer;
//} DATA_LIST;

//typedef struct
//{
//	TABLE_LIST tables;
//	DATA_LIST data;
//} KERNEL_INFO;

//char stack[0x1000];

//int _kernel_start(KERNEL_INFO* info){
	

//	VOID *FrameBufferBase = (void*)info->data.screenBuffer;
//	UINT32 Width = info->data.gInfo.HorizontalResolution;
//	UINT32 Height = info->data.gInfo.VerticalResolution;
//	UINT32 PixelsPerScanLine = info->data.gInfo.PixelsPerScanLine;

//	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *FB = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)FrameBufferBase;

//	// 画面を真っ赤に塗る
//	for (UINTN y = 0; y < Height; y++) {
//		for (UINTN x = 0; x < Width; x++) {
//			FB[y * PixelsPerScanLine + x].Red = 255;
//			FB[y * PixelsPerScanLine + x].Green = 0;
//			FB[y * PixelsPerScanLine + x].Blue = 255;
//			FB[y * PixelsPerScanLine + x].Reserved = 0;
//		}
//	}

//	while (1)
//	{
//		asm volatile ("hlt\n\t");
//	}
	

//	return 0;
//}