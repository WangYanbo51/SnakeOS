#ifndef _BOOT_H_
#define _BOOT_H_

#include "type.h"

#pragma pack(1)
typedef struct
{
	VOID *Buffer;
	UINTN MapSize;
	UINTN MapKey;
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;
} MEMORY_MAP;
typedef struct
{
	UINT64 FrameBufferBase;
	UINT64 FrameBufferSize;
	UINT32 PixelsPerScanLine;
	UINT32 HorizontalResolution;
	UINT32 VerticalResolution;
} VIDEO_CONFIG;
typedef struct
{
	VIDEO_CONFIG VideoConfig;
	MEMORY_MAP   MemoryMap;
	EFI_PHYSICAL_ADDRESS KernelEntryPoint;
} BOOT_CONFIG;
#pragma pack()

typedef UINT64 (__attribute__((sysv_abi)) *KERNEL_ENTRY)(BOOT_CONFIG *BootConfig);
// typedef UINT64 (*KERNEL_ENTRY)(BOOT_CONFIG *BootConfig);
#endif
