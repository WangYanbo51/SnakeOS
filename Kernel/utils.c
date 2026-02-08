#include "utils.h"

void sys_memset64(void *dest, UINT64 val, UINT64 count)
{
	asm volatile (
		"rep stosq"
		: "+D"(dest), "+c"(count)
		: "a"(val)
		: "memory"
	);
}
void sys_memset32(void *dest, UINT32 val, UINT64 count)
{
	asm volatile (
		"rep stosl"
		: "+D"(dest), "+c"(count)
		: "a"(val)
		: "memory"
	);
}
void sys_memcpy64(void *dest, const void *src, UINT64 n_bytes)
{
	UINT64 count = n_bytes / 16;

	__asm__ __volatile__ (
		"1: \n\t"
		"movaps (%1), %%xmm0 \n\t"
		"movntps %%xmm0, (%0) \n\t"
		"add $16, %0 \n\t"
		"add $16, %1 \n\t"
		"dec %2 \n\t"
		"jnz 1b \n\t"
		"sfence \n\t"
		: "+r"(dest), "+r"(src), "+r"(count)
		:
		: "xmm0", "memory"
	);
}
void sys_memcpy32(void *dest, const void *src, UINT64 count)
{
	asm volatile (
		"rep movsl"
		: "+D"(dest), "+S"(src), "+c"(count)
		:
		: "memory"
	);
}
void itoa(UINT64 n, char *str, int base)
{
	static char chars[] = "0123456789ABCDEF";
	int i = 0;
	if (n == 0)
	{
		str[i++] = '0';
	}
	else
	{
		while (n > 0)
		{
			str[i++] = chars[n % base];
			n /= base;
		}
		for (int j = 0; j < (i >> 1); j++)
		{
			char tmp = str[j];
			str[j] = str[i - j - 1];
			str[i - j - 1] = tmp;
		}
	}
	str[i] = '\0';
}
