#include "draw.h"
#include "log.h"

EFI_STATUS GetGOPHandle(IN EFI_HANDLE ImageHandle, OUT EFI_GRAPHICS_OUTPUT_PROTOCOL **Gop)
{
	EFI_STATUS Status = EFI_SUCCESS;
	UINTN NoHandles = 0;
	EFI_HANDLE *Buffer = NULL;
	Status = gBS->LocateHandleBuffer(
				 ByProtocol,
				 &gEfiGraphicsOutputProtocolGuid,
				 NULL,
				 &NoHandles,
				 &Buffer
			 );
	// if (EFI_ERROR(Status))
	// 	return Status;
	LOG(L"Failed to GetGOPHandle: LocateHandleBuffer.\r\n");
	Status = gBS->OpenProtocol(
				 Buffer[0],
				 &gEfiGraphicsOutputProtocolGuid,
				 (VOID **)Gop,
				 ImageHandle,
				 NULL,
				 EFI_OPEN_PROTOCOL_GET_PROTOCOL
			 );
	LOG(L"Failed to GetGOPHandle: OpenProtocol.\r\n");
	return Status;
}
EFI_STATUS DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINTN x, UINTN y, UINTN w, UINTN h, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Color)
{
	EFI_STATUS Status = EFI_SUCCESS;
	Gop->Blt(
		Gop,
		Color,
		EfiBltVideoFill,
		0, 0,
		x, y,
		w, h,
		0
	);
	return Status;
}

EFI_STATUS QueryAllModes(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop)
{
	EFI_STATUS Status = EFI_SUCCESS;
	UINTN SizeOfInfo = 0;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
	for (UINTN i = 0; i < Gop->Mode->MaxMode; i++)
	{
		Status = Gop->QueryMode(Gop, i, &SizeOfInfo, &Info);
		LOG(L"Failed to QueryMode.\r\n");
		Print(L"Mode %d, N = %d, V = %d\r\n", i, Info->HorizontalResolution, Info->VerticalResolution);
	}
	return Status;
}

