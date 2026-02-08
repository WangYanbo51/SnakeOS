#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "../Inc/type.h"

#define KB_DATA_PORT    0x60
#define KB_STATUS_PORT  0x64
#define KB_STATUS_OBF   0x01

typedef enum
{
  KEY_NONE,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_ESC
} KEY_EVENT;

UINT8 inb(UINT16 port);
void outb(UINT16 port, UINT8 data);
UINT8 wait_kb();

KEY_EVENT get_kb_event();

#endif // _KEYBOARD_H_
