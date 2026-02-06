#ifndef _LOG_H_
#define _LOG_H_

#define DEBUG

#ifdef DEBUG
#define LOG(A) {if (EFI_ERROR(Status)) { Print(A); return Status;}}
#define LOG_STATUS(A) {if (EFI_ERROR(Status)) { Print(A, Status); return Status;}}
#else
#define LOG(A) {if (EFI_ERROR(Status)) { return Status;}}
#define LOG_STATUS(A) {if (EFI_ERROR(Status)) { return Status;}}
#endif

#endif  // _LOG_H_
