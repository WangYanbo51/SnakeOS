#ifndef _ELF_H_
#define _ELF_H_

#define ELF_MAGIC     0x464c457F

#define ELF_32        1
#define ELF_64        2
#define NOT_ELF_FILE  -1
#define NOT_64_BITS    -2
#define PT_LOAD       1

#pragma pack(1)
typedef struct
{
	UINT32 Magic;
	UINT8  Format;
	UINT8  Endianness;
	UINT8  Version;
	UINT8  OSAbi;
	UINT8  AbiVersion;
	UINT8  Reserved[7];
	UINT16 Type;
	UINT16 Machine;
	UINT32 ElfVersion;
	UINT64 Entry;
	UINT64 Phoff;
	UINT64 Shoff;
	UINT32 Flags;
	UINT16 HeadSize;
	UINT16 PHeadSize;
	UINT16 PHeadCount;
	UINT16 SHeadSize;
	UINT16 SHeadCount;
	UINT16 SNameIndex;
} ELF_HEADER_64;
typedef struct
{
	UINT32 Type;
	UINT32 Flags;
	UINT64 Offset;
	UINT64 VAddress;
	UINT64 PAddress;
	UINT64 SizeInFile;
	UINT64 SizeInMemory;
	UINT64 Align;
} PROGRAM_HEADER_64;
#pragma pack()

EFI_STATUS GetElfEntry(
	IN EFI_HANDLE ImageHandle,
	IN CHAR16 *FileName,
	OUT EFI_PHYSICAL_ADDRESS *KernelEntry);
EFI_STATUS CheckELF(
	IN EFI_PHYSICAL_ADDRESS KernelBuffer);
EFI_STATUS LoadSegments(
	IN EFI_PHYSICAL_ADDRESS KernelBufferBase,
	OUT EFI_PHYSICAL_ADDRESS *KernelEntry);

UINTN GetValue(
	IN EFI_PHYSICAL_ADDRESS StartAddress,
	IN UINTN Offset,
	IN UINTN Size
);

#endif  // _ELF_H_
