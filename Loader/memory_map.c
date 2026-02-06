#include "memory_map.h"
#include "log.h"

EFI_STATUS GetMapAndExit(EFI_HANDLE ImageHandle, MEMORY_MAP *MemoryMap)
{
	EFI_STATUS Status = EFI_SUCCESS;

	Status = gBS->AllocatePool(EfiLoaderData, MemoryMap->MapSize, &MemoryMap->Buffer);
	LOG(L"Failed to allocate memory to get memory map.\r\n");

	Status = gBS->GetMemoryMap(
				 &MemoryMap->MapSize,
				 (EFI_MEMORY_DESCRIPTOR *)MemoryMap->Buffer,
				 &MemoryMap->MapKey,
				 &MemoryMap->DescriptorSize,
				 &MemoryMap->DescriptorVersion
			 );
	if (Status == EFI_BUFFER_TOO_SMALL)
	{
		gBS->FreePool(MemoryMap->Buffer);
		MemoryMap->MapSize += (MemoryMap->DescriptorSize << 1);
		Status = gBS->AllocatePool(EfiLoaderData, MemoryMap->MapSize, &MemoryMap->Buffer);
		Status = gBS->GetMemoryMap(
					 &MemoryMap->MapSize,
					 (EFI_MEMORY_DESCRIPTOR *)MemoryMap->Buffer,
					 &MemoryMap->MapKey,
					 &MemoryMap->DescriptorSize,
					 &MemoryMap->DescriptorVersion
				 );
		LOG_STATUS(L"Failed to GetMemoryMap, %d.\r\n");
	}
	Status = gBS->ExitBootServices(ImageHandle, MemoryMap->MapKey);
	// LOG(L"Could not exit boot services.\r\n");
	return Status;
}
