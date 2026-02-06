#include "SnakeOS.h"
#include "draw.h"
#include "elf.h"
#include "../Inc/boot.h"
#include "memory_map.h"
#include "log.h"

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS Status = EFI_SUCCESS;

	// Get graphic output device handle
	EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
	Status = GetGOPHandle(ImageHandle, &Gop);
  LOG(L"Failed to GetGOPHandle.\r\n");

	// Query and set graphic mode
	// Status = QueryAllModes(Gop);
	Status = Gop->SetMode(Gop, Gop->Mode->MaxMode - 1);
	LOG(L"Failed to set mode.\r\n");

	// Print hello
	gST->ConOut->ClearScreen(gST->ConOut);
	Print(L"Hello, SnakeOS Loader!\r\n");
	// gBS->Stall(2000000);

	// Drawing...
	// EFI_GRAPHICS_OUTPUT_BLT_PIXEL Red = {0, 0, 255, 0};
	// EFI_GRAPHICS_OUTPUT_BLT_PIXEL Blue = {255, 0, 0, 0};
	// DrawRect(Gop, 0, 50, 100, 100, &Red);
	// DrawRect(Gop, 0, 0, Gop->Mode->Info->PixelsPerScanLine, Gop->Mode->Info->VerticalResolution, &Blue);
	// Copy the red rect (0, 50) to (200, 250)
	// Gop->Blt(
	// 	Gop,
	// 	&Red,
	// 	EfiBltVideoToVideo,     // Copy mode
	// 	0, 50,
	// 	200, 250,
	// 	100, 100,
	// 	0
	// );

	/////// Another way to fill a blue tape on screen ///////
	/// By directly fill {BLUE} in memory (*VideoHandle) ////
	// EFI_PHYSICAL_ADDRESS BlueStart = Gop->Mode->FrameBufferBase
	// 								 + Gop->Mode->Info->PixelsPerScanLine * 150 * 4;
	// EFI_GRAPHICS_OUTPUT_BLT_PIXEL *VideoHandle = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) BlueStart;
	// UINTN NoPixels = Gop->Mode->Info->PixelsPerScanLine * 100;
	// for (UINTN i = 0; i < NoPixels; i++)
	// {
	// 	*VideoHandle = Red;
	// 	VideoHandle++;
	// }

	// Load kernel in memory
	// EFI_FILE_PROTOCOL *Bin;
	// Status = GetFileHandle(ImageHandle, L"\\kernel.bin", &Bin);
	// EFI_PHYSICAL_ADDRESS BinAddress;
	// Status = ReadFile(Bin, &BinAddress);

	// Load ELF kernel in memory
	EFI_PHYSICAL_ADDRESS KernelEntryPoint;
	Status = GetElfEntry(ImageHandle, L"\\kernel.elf", &KernelEntryPoint);
  LOG(L"Failed to get ELF entry.\r\n");

	// gBS->Stall(2000000);
	// setting custom boot arguments
	BOOT_CONFIG BootConfig;
	BootConfig.MemoryMap.MapSize = 2048;
	BootConfig.MemoryMap.Buffer = NULL;
	BootConfig.MemoryMap.MapKey = 0;
  BootConfig.MemoryMap.DescriptorSize = 0;
	BootConfig.MemoryMap.DescriptorVersion = 0;

  BootConfig.VideoConfig.FrameBufferBase = Gop->Mode->FrameBufferBase;
  BootConfig.VideoConfig.FrameBufferSize = Gop->Mode->FrameBufferSize;
  BootConfig.VideoConfig.PixelsPerScanLine = Gop->Mode->Info->PixelsPerScanLine;
  BootConfig.VideoConfig.HorizontalResolution = Gop->Mode->Info->HorizontalResolution;
  BootConfig.VideoConfig.VerticalResolution = Gop->Mode->Info->VerticalResolution;

  Print(L"Now Exiting Boot Services...\r\n");
  Status = GetMapAndExit(ImageHandle, &BootConfig.MemoryMap);
  // LOG(L"Could not GetMapAndExit.\r\n");
	// If all set, jump to the kernel
	// Print(L"All Success! Now jump to Kernel!\n\r");

	// gBS->Stall(1000000);

	// weird jumping method to the kernel made with NASM
	// asm volatile (
	// 	"movq %0, %%rdi\n\t"
	// 	"cli\n\t"
	// 	"jmp *%1"
	// 	:
	// 	: "r"(Gop->Mode->FrameBufferBase), "r"(BinAddress)
	// 	: "rdi"
	// );

	// A greater way to call kernel made with C language
	// KERNEL_ENTRY kernel_main = (KERNEL_ENTRY)BinAddress;
	KERNEL_ENTRY kernel_main = (KERNEL_ENTRY)KernelEntryPoint;
	// After exited boot services, we can no longer functions link Print().
  // UINT64 ret = kernel_main(&BootConfig);
	// Print(L"ret = 0x%llX\r\n", ret);
	// while(1) {}
	kernel_main(&BootConfig);
  return Status;
}
