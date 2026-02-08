#include "time.h"

inline UINT64 rdtsc() {
    UINT32 low, high;
    __asm__ __volatile__("rdtsc" : "=a"(low), "=d"(high));
    return ((UINT64)high << 32) | low;
}

void delay_cycles(UINT64 cycles) {
    UINT64 start = rdtsc();
    while ((rdtsc() - start) < cycles);
}
