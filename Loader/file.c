#include "file.h"

EFI_STATUS GetFileHandle(EFI_HANDLE ImageHandle, CHAR16 *FileName, EFI_FILE_PROTOCOL **FileHandle)
{
	EFI_STATUS Status = EFI_SUCCESS;
	// UINTN HandleCount = 0;
	// EFI_HANDLE *HandleBuffer;
	EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
	// Status = gBS->LocateHandleBuffer(
	// 			 ByProtocol,
	// 			 &gEfiSimpleFileSystemProtocolGuid,
	// 			 NULL,
	// 			 &HandleCount,
	// 			 &HandleBuffer
	// 		 );
	Status = gBS->HandleProtocol(
				 ImageHandle,
				 &gEfiLoadedImageProtocolGuid,
				 (VOID **)&LoadedImage
			 );
	if (EFI_ERROR(Status))
	{
		// Print(L"Failed to 1.\r\n");
		return Status;
	}
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
	// Status = gBS->OpenProtocol(
	// 			 HandleBuffer[0],
	// 			 &gEfiSimpleFileSystemProtocolGuid,
	// 			 (VOID **)&FileSystem,
	// 			 ImageHandle,
	// 			 NULL,
	// 			 EFI_OPEN_PROTOCOL_GET_PROTOCOL
	// 		 );
	Status = gBS->HandleProtocol(
				 LoadedImage->DeviceHandle,
				 &gEfiSimpleFileSystemProtocolGuid,
				 (VOID **)&FileSystem
			 );
	if (EFI_ERROR(Status))
	{
		// Print(L"Failed to 2.\r\n");
		return Status;
	}
	EFI_FILE_PROTOCOL *Root;
	Status = FileSystem->OpenVolume(
				 FileSystem,
				 &Root
			 );
	if (EFI_ERROR(Status))
	{
		// Print(L"Failed to 3.\r\n");
		return Status;
	}
	Status = Root->Open(
				 Root,
				 FileHandle,
				 FileName,
				 EFI_FILE_MODE_READ,
				 EFI_OPEN_PROTOCOL_GET_PROTOCOL
			 );
	return Status;
}

EFI_STATUS ReadFile(EFI_FILE_PROTOCOL *File, EFI_PHYSICAL_ADDRESS *FileBase)
{
	EFI_STATUS Status = EFI_SUCCESS;
	EFI_FILE_INFO *FileInfo;

	UINTN InfoSize = sizeof(EFI_FILE_INFO) + 128;
	Status = gBS->AllocatePool(
				 EfiLoaderData,
				 InfoSize,
				 (VOID **)&FileInfo
			 );
	if (EFI_ERROR(Status))
	{
		// Print(L"Failed to 5.\r\n");
		return Status;
	}
	Status = File->GetInfo(
				 File,
				 &gEfiFileInfoGuid,
				 &InfoSize,
				 FileInfo
			 );
	if (EFI_ERROR(Status))
	{
		// Print(L"Failed to 6.\r\n");
		return Status;
	}
	UINTN FilePageSize = (FileInfo->FileSize >> 12) + 1;

	EFI_PHYSICAL_ADDRESS FileBufferAddress;
	Status = gBS->AllocatePages(
				 AllocateAnyPages,
				 EfiLoaderData,
				 FilePageSize,
				 &FileBufferAddress
			 );
	if (EFI_ERROR(Status))
	{
		// Print(L"Failed to 7.\r\n");
		return Status;
	}
	UINTN ReadSize = FileInfo->FileSize;
	Status = File->Read(
				 File,
				 &ReadSize,
				 (VOID *)FileBufferAddress
			 );
	if (EFI_ERROR(Status))
	{
		// Print(L"Failed to 8.\r\n");
		return Status;
	}
	gBS->FreePool(FileInfo);
	*FileBase = FileBufferAddress;
	return Status;
}
