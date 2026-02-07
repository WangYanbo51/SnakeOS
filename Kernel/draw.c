#include "draw.h"
#include "font.h"
#include "utils.h"
#include <stdarg.h>

__attribute__((aligned(16)))
UINT32 back_buffer[MAX_RESOLUTION_HEIGHT * MAX_RESOLUTION_WIDTH];

UINT32 HResolution;
UINT32 VResolution;

UINT32 *render_target;
VOID *vram;   //FrameBufferBase
UINT32 ppsl;  //PixelsPerScanLine
UINT32 buffer_size;

void draw_init(VIDEO_CONFIG VideoConfig)
{
	vram = (VOID *)VideoConfig.FrameBufferBase;
	ppsl = VideoConfig.PixelsPerScanLine;
	render_target = back_buffer;
	HResolution = VideoConfig.HorizontalResolution;
	VResolution = VideoConfig.VerticalResolution;
	buffer_size = VideoConfig.FrameBufferSize;
}
void printf_at(int x, int y, UINT32 color, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	int cur_x = x;
	char buf[64];

	while (*fmt)
	{
		if (*fmt == '%' && *(fmt + 1))
		{
			fmt++;
			switch (*fmt)
			{
			case 'd':
			{
				int n = va_arg(args, int);
				if (n < 0)
				{
					put_char('-', cur_x, y, color);
					cur_x += font_width;
					n = -n;
				}
				itoa((UINT64)n, buf, 10);
				for (int i = 0; buf[i]; i++)
				{
					put_char(buf[i], cur_x, y, color);
					cur_x += font_width;
				}
				break;
			}
			case 'x':
			{
				UINT64 n = va_arg(args, UINT64);
				itoa(n, buf, 16);
				put_char('0', cur_x, y, color);
				cur_x += font_width;
				put_char('x', cur_x, y, color);
				cur_x += font_width;
				for (int i = 0; buf[i]; i++)
				{
					put_char(buf[i], cur_x, y, color);
					cur_x += font_width;
				}
				break;
			}
			case 's':
			{
				char *s = va_arg(args, char *);
				while (*s)
				{
					put_char(*s++, cur_x, y, color);
					cur_x += font_width;
				}
				break;
			}
			case 'c':
			{
				char c = (char)va_arg(args, int);
				put_char(c, cur_x, y, color);
				cur_x += font_width;
				break;
			}
			default:
				put_char(*fmt, cur_x, y, color);
				cur_x += font_width;
				break;
			}
		}
		else
		{
			put_char(*fmt, cur_x, y, color);
			cur_x += font_width;
		}
		fmt++;
	}
	va_end(args);
}

void draw_pixel(int x, int y, UINT32 color)
{
	if (x >= 0 && x < HResolution && y >= 0 && y < VResolution)
		render_target[y * ppsl + x] = color;
}

void fill_screen(UINT32 color)
{
	sys_memset64(back_buffer, ((UINT64)color << 32) | color, buffer_size >> 3);
}
void show_frame()
{
	sys_memcpy64(vram, back_buffer, buffer_size);
}
void draw_rect(int x, int y, UINT32 w, UINT32 h, UINT32 color)
{
	int x1 = (x < 0) ? 0 : x;
	int y1 = (y < 0) ? 0 : y;
	int x2 = (x + w > HResolution) ? HResolution : (x + w);
	int y2 = (y + h > VResolution) ? VResolution : (y + h);

	if (x1 >= x2 || y1 >= y2) return;

	int fill_width = x2 - x1;
	int fill_height = y2 - y1;

	UINT32 *dest = &back_buffer[y1 * ppsl + x1];

	for (int i = 0; i < fill_height; i++)
	{
		for (int j = 0; j < fill_width; j++)
		{
			dest[j] = color;
		}

		dest += ppsl;
	}
}
