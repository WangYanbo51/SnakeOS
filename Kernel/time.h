#ifndef _TIME_H_
#define _TIME_H_

#include "../Inc/type.h"

inline UINT64 rdtsc();
void delay_cycles(UINT64 cycles);

#endif // _TIME_H_
