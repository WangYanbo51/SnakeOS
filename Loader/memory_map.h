#ifndef _MEMORY_MAP_H_
#define _MEMORY_MAP_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "../Inc/boot.h"

EFI_STATUS GetMapAndExit(IN EFI_HANDLE ImageHandle, OUT MEMORY_MAP *MemoryMap);

#endif  // _MEMORY_MAP_H_
