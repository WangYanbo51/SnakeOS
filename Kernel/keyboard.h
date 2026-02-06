#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "../Inc/boot.h"

#define KB_DATA_PORT    0x60
#define KB_STATUS_PORT  0x64
#define KB_STATUS_OBF   0x01

UINT8 inb(UINT16 port);
UINT8 get_kb();

#endif // _KEYBOARD_H_
