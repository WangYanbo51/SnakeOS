#include "keyboard.h"

UINT8 inb(UINT16 port)
{
	UINT8 data;
	__asm__ volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
	return data;
}

void outb(UINT16 port, UINT8 data)
{
  __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

KEY_EVENT get_kb_event()
{
  if (!(inb(KB_STATUS_PORT) & KB_STATUS_OBF)) {
    return KEY_NONE;
  }
  UINT8 scancode = inb(KB_DATA_PORT);

  if (scancode == 0xE0)
  {
    while(!(inb(KB_STATUS_PORT) & KB_STATUS_OBF)) {
      __asm__ volatile("pause");
    }
    scancode = inb(KB_DATA_PORT);

    switch (scancode)
    {
      case 0x48:
        return KEY_UP;
      case 0x50:
        return KEY_DOWN;
      case 0x4B:
        return KEY_LEFT;
      case 0x4D:
        return KEY_RIGHT;
      default:
        return KEY_NONE;
    }
  }
  switch (scancode)
  {
    case 0x11: return KEY_UP; // W
    case 0x1F: return KEY_DOWN; // S
    case 0x1E: return KEY_LEFT; // A
    case 0x20: return KEY_RIGHT; // D
  }
  return KEY_NONE;
}

UINT8 wait_kb()
{
    UINT8 scancode;

    while (1)
    {
        while (!(inb(KB_STATUS_PORT) & KB_STATUS_OBF)) {
            __asm__ volatile("pause");
        }
        scancode = inb(KB_DATA_PORT);
        if (!(scancode & 0x80)) {
            return scancode;
        }
    }
}
