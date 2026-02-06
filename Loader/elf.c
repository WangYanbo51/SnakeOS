#include "elf.h"
#include "file.h"

EFI_STATUS GetElfEntry(IN EFI_HANDLE ImageHandle, IN CHAR16 *FileName, OUT EFI_PHYSICAL_ADDRESS *KernelEntry)
{
	EFI_STATUS Status = EFI_SUCCESS;
	EFI_FILE_PROTOCOL *Kernel;
	Status = GetFileHandle(ImageHandle, FileName, &Kernel);
	if (EFI_ERROR(Status))
		return Status;
	EFI_PHYSICAL_ADDRESS KernelBuffer;
	Status = ReadFile(Kernel, &KernelBuffer);
	if (EFI_ERROR(Status))
		return Status;
	Status = CheckELF(KernelBuffer);
	if (EFI_ERROR(Status))
		return Status;
	Status = LoadSegments(KernelBuffer, KernelEntry);

	return Status;
}
EFI_STATUS CheckELF(IN EFI_PHYSICAL_ADDRESS KernelBuffer)
{
	EFI_STATUS Status = EFI_SUCCESS;
	UINT32 Magic = GetValue(KernelBuffer, 0x00, 4);
	if (Magic != ELF_MAGIC)
	{
		Print(L"ERROR: Not a ELF file.\r\n");
		Status = NOT_ELF_FILE;
	}
	UINT8 Format = GetValue(KernelBuffer, 0x04, 1);
	if (Format != ELF_64)
	{
		Status = NOT_64_BITS;
		return Status;
	}
	return Status;
}
EFI_STATUS LoadSegments(
	IN EFI_PHYSICAL_ADDRESS KernelBufferBase,
	OUT EFI_PHYSICAL_ADDRESS *KernelEntry
)
{
	EFI_STATUS Status = EFI_SUCCESS;

	ELF_HEADER_64 *ElfHeader = (ELF_HEADER_64 *)KernelBufferBase;
	PROGRAM_HEADER_64 *PHeader = (PROGRAM_HEADER_64 *)(KernelBufferBase + ElfHeader->Phoff);

	EFI_PHYSICAL_ADDRESS LowAddr = 0xFFFFFFFFFFFFFFFF;
	EFI_PHYSICAL_ADDRESS HighAddr = 0;
	// Print(L"ELF Header Entry: %p, PHoff: %d, PHeadCount: %d\n", ElfHeader->Entry, ElfHeader->Phoff, ElfHeader->PHeadCount);
	for (UINTN i = 0; i < ElfHeader->PHeadCount; i++)
	{
		if (PHeader[i].Type == PT_LOAD)
		{
			// Print(L"Segment %d: Offset 0x%x, PAddr 0x%lx, SizeInFile 0x%x\n",
				  // i, PHeader[i].Offset, PHeader[i].PAddress, PHeader[i].SizeInFile);
			if (LowAddr > PHeader[i].PAddress)
        LowAddr = PHeader[i].PAddress;
			if (HighAddr < (PHeader[i].PAddress + PHeader[i].SizeInMemory))
			{
				HighAddr = PHeader[i].PAddress + PHeader[i].SizeInMemory;
			}
		}
	}

	UINTN PageCount = (HighAddr - LowAddr + 0xFFF) >> 12;
	EFI_PHYSICAL_ADDRESS KernelRelocateBase;
	Status = gBS->AllocatePages(
				 AllocateAnyPages,
				 EfiLoaderCode,
				 // EfiReservedMemoryType,
				 PageCount,
				 &KernelRelocateBase);

	if (EFI_ERROR(Status)) return Status;

	UINT64 RelocateOffset = KernelRelocateBase - LowAddr;

	for (UINTN i = 0; i < ElfHeader->PHeadCount; i++)
	{
		if (PHeader[i].Type == PT_LOAD)
		{
			UINT8 *DestStart = (UINT8 *)(PHeader[i].PAddress + RelocateOffset);
			UINT8 *SourceStart = (UINT8 *)(KernelBufferBase + PHeader[i].Offset);

			if (PHeader[i].SizeInFile > 0)
				gBS->CopyMem(DestStart, SourceStart, PHeader[i].SizeInFile);

			if (PHeader[i].SizeInMemory > PHeader[i].SizeInFile)
			{
				UINTN BssSize = PHeader[i].SizeInMemory - PHeader[i].SizeInFile;
				UINT8 *BssStart = DestStart + PHeader[i].SizeInFile;
				gBS->SetMem(BssStart, BssSize, 0);
			}
		}
	}

	*KernelEntry = ElfHeader->Entry + RelocateOffset;
	return Status;
}
// EFI_STATUS LoadSegments(
// 	IN EFI_PHYSICAL_ADDRESS KernelBufferBase,
// 	OUT EFI_PHYSICAL_ADDRESS *KernelEntry
// )
// {
// 	EFI_STATUS Status = EFI_SUCCESS;
//
// 	ELF_HEADER_64 *ElfHeader = (ELF_HEADER_64 *)KernelBufferBase;
// 	PROGRAM_HEADER_64 *PHeader = (PROGRAM_HEADER_64 *)(KernelBufferBase + ElfHeader->Phoff);
//
// 	EFI_PHYSICAL_ADDRESS LowAddr = 0xFFFFFFFFFFFFFFFF;
// 	EFI_PHYSICAL_ADDRESS HighAddr = 0;
//
// 	for (UINTN i = 0; i < ElfHeader->PHeadCount; i++)
// 	{
// 		if (PHeader[i].Type == PT_LOAD)
// 		{
// 			if (LowAddr > PHeader[i].PAddress)
// 			{
// 				LowAddr = PHeader[i].PAddress;
// 			}
// 			if (HighAddr < (PHeader[i].PAddress + PHeader[i].SizeInMemory))
// 			{
// 				HighAddr = PHeader[i].PAddress + PHeader[i].SizeInMemory;
// 			}
// 		}
// 	}
//
// 	UINTN PageCount = ((HighAddr - LowAddr + 0xFFF) >> 12);
// 	EFI_PHYSICAL_ADDRESS KernelRelocateBase;
// 	Status = gBS->AllocatePages(
// 				 AllocateAnyPages,
// 				 EfiLoaderCode,
// 				 PageCount,
// 				 &KernelRelocateBase);
//
//
// 	if(EFI_ERROR(Status))
// 		return Status;
//
// 	UINT64 RelocateOffset = KernelRelocateBase - LowAddr;
// 	UINT64 *ZeroStart = (UINT64 *)KernelRelocateBase;
// 	for(UINTN i = 0; i < (PageCount << 9); i++)
// 	{
// 		*ZeroStart = 0x000000000000;
// 		ZeroStart++;
// 	}
// 	// gBS->SetMem((VOID*)KernelRelocateBase, (PageCount << 9), 0);
//
// 	for (UINTN i = 0; i < ElfHeader->PHeadCount; i++)
// 	{
//
// 		if (PHeader[i].Type == PT_LOAD)
// 		{
// 			UINT8 *SourceStart = (UINT8 *)KernelBufferBase + PHeader[i].Offset;
// 			UINT8 *DestStart = (UINT8 *)PHeader[i].VAddress + RelocateOffset;
// 			// gBS->CopyMem(DestStart, SourceStart, PHeader[i].SizeInFile);
// 			for(UINTN j = 0; j < PHeader[i].SizeInFile; j++)
// 			{
// 				*DestStart = *SourceStart;
// 				SourceStart++;
// 				DestStart++;
// 			}
// 		}
// 	}
// 	*KernelEntry = ElfHeader->Entry + RelocateOffset;
//
// 	return Status;
// }

UINTN GetValue(
	IN EFI_PHYSICAL_ADDRESS StartAddress,
	IN UINTN Offset,
	IN UINTN Size
)
{
	UINT8 *ByteStart = (UINT8 *)StartAddress + Offset;
	UINTN Result = 0;
	for(UINTN i = 0; i < Size; i++)
	{
		Result += *(ByteStart + i) << i * 8;
	}
	return Result;
}
