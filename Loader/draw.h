#ifndef _DRAW_H_
#define _DRAW_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Guid/FileInfo.h>

EFI_STATUS GetGOPHandle(
	IN EFI_HANDLE ImageHandle,
	OUT EFI_GRAPHICS_OUTPUT_PROTOCOL **Gop);

EFI_STATUS DrawRect(
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
	IN UINTN x, IN UINTN y,
	IN UINTN w, IN UINTN h,
	IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Color);

EFI_STATUS QueryAllModes(
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop);

#endif
