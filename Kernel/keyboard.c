#include "keyboard.h"

UINT8 inb(UINT16 port)
{
	UINT8 data;
	__asm__ volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
	return data;
}

// UINT8 get_kb()
// {
// 	UINT8 status;
//
// 	while (1)
// 	{
// 		status = inb(KB_STATUS_PORT);
// 		if (status & KB_STATUS_OBF)
// 			break;
// 		__asm__ volatile("pause");
// 	}
// 	return inb(KB_DATA_PORT);
// }
UINT8 get_kb()
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
