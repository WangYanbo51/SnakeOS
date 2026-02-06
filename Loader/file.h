#ifndef _FILE_H_
#define _FILE_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Guid/FileInfo.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>

EFI_STATUS GetFileHandle(IN EFI_HANDLE ImageHandle, IN CHAR16 *FileName, OUT EFI_FILE_PROTOCOL **FileHandle);
EFI_STATUS ReadFile(IN EFI_FILE_PROTOCOL *File,
                    OUT EFI_PHYSICAL_ADDRESS *FileBase);

#endif  // _FILE_H_
