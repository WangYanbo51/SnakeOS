#ifndef _DRAW_H_
#define _DRAW_H_

#include "../Inc/type.h"
#include "../Inc/boot.h"

#define MAX_RESOLUTION_WIDTH    2560
#define MAX_RESOLUTION_HEIGHT   1600

extern UINT32 HResolution;
extern UINT32 VResolution;

extern VOID *vram;  //FrameBufferBase
extern UINT32 ppsl; //PixelsPerScanLine
extern UINT32 back_buffer[MAX_RESOLUTION_WIDTH * MAX_RESOLUTION_HEIGHT];

void draw_init(VIDEO_CONFIG VideoConfig);
void draw_pixel(UINT32 x, UINT32 y, UINT32 color);
void fill_screen(UINT32 color);
void printf_at(UINT32 x, UINT32 y, UINT32 color, const char *fmt, ...);
void show_frame();
void draw_rect(UINT32 x, UINT32 y, UINT32 w, UINT32 h, UINT32 color);

#endif // _DRAW_H_
