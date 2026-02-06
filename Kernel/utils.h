#ifndef _UTILS_H_
#define _UTILS_H_

#include "../Inc/type.h"

void itoa(UINT64 n, char *str, int base);
void sys_memset64(void *dest, UINT64 val, UINT64 count);
void sys_memcpy64(void *dest, const void *src, UINT64 n_bytes);

#endif // _UTILS_H_
