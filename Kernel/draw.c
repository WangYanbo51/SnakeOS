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

static int dirty_x1, dirty_y1, dirty_x2, dirty_y2;
static int has_dirty = 0;

void mark_dirty(int x, int y, int w, int h)
{
	int x2 = x + w;
	int y2 = y + h;

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x2 > (int)HResolution) x2 = (int)HResolution;
	if (y2 > (int)VResolution) y2 = (int)VResolution;
	if (x >= x2 || y >= y2) return;

	if (!has_dirty)
	{
		dirty_x1 = x;
		dirty_y1 = y;
		dirty_x2 = x2;
		dirty_y2 = y2;
		has_dirty = 1;
	}
	else
	{
		if (x < dirty_x1) dirty_x1 = x;
		if (y < dirty_y1) dirty_y1 = y;
		if (x2 > dirty_x2) dirty_x2 = x2;
		if (y2 > dirty_y2) dirty_y2 = y2;
	}
}
void Optimize(UINT64 base)
{
	UINT32 base_low = (UINT32)(base | 0x01);
	UINT32 base_high = (UINT32)(base >> 32);
	UINT64 mask_val = 0xFFFFFFF000ULL | 0x800;
	UINT32 mask_low = (UINT32)mask_val;
	UINT32 mask_high = (UINT32)(mask_val >> 32);

	__asm__ volatile("wrmsr" : : "a"(base_low), "d"(base_high), "c"(0x200));
	__asm__ volatile("wrmsr" : : "a"(mask_low), "d"(mask_high), "c"(0x201));
}

void draw_init(VIDEO_CONFIG VideoConfig)
{
	vram = (VOID *)VideoConfig.FrameBufferBase;
	ppsl = VideoConfig.PixelsPerScanLine;
	// render_target = (UINT32 *)vram;
	render_target = back_buffer;
	HResolution = VideoConfig.HorizontalResolution;
	VResolution = VideoConfig.VerticalResolution;
	buffer_size = VideoConfig.FrameBufferSize;
	Optimize((UINT64)vram);
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
	if (x >= 0 && x < (int)HResolution && y >= 0 && y < (int)VResolution)
		render_target[y * ppsl + x] = color, mark_dirty(x, y, 1, 1);
}

void fill_screen(UINT32 color)
{
	sys_memset64(render_target, ((UINT64)color << 32) | color, buffer_size >> 3);
	mark_dirty(0, 0, HResolution, VResolution);
}
void show_frame()
{
	if (!has_dirty) return;

	if (dirty_x1 < 0) dirty_x1 = 0;
	if (dirty_y1 < 0) dirty_y1 = 0;
	if (dirty_x2 > (int)HResolution) dirty_x2 = (int)HResolution;
	if (dirty_y2 > (int)VResolution) dirty_y2 = (int)VResolution;

	int width_to_copy = dirty_x2 - dirty_x1;
	int height_to_copy = dirty_y2 - dirty_y1;

	if (width_to_copy > 0 && height_to_copy > 0)
	{
		UINT32 *src = &back_buffer[dirty_y1 * ppsl + dirty_x1];
		UINT32 *dst = (UINT32 *)vram + (dirty_y1 * ppsl + dirty_x1);

		for (int i = 0; i < height_to_copy; i++)
		{
			sys_memcpy32(dst, src, width_to_copy);

			src += ppsl;
			dst += ppsl;
		}
	}
	__asm__ volatile("sfence" ::: "memory");
	has_dirty = 0;
}
// void show_frame()
// {
// 	sys_memcpy64(vram, back_buffer, buffer_size);
// }
// void draw_rect(int x, int y, UINT32 w, UINT32 h, UINT32 color)
// {
// 	int x1 = (x < 0) ? 0 : x;
// 	int y1 = (y < 0) ? 0 : y;
// 	int x2 = (x + w > HResolution) ? HResolution : (x + w);
// 	int y2 = (y + h > VResolution) ? VResolution : (y + h);
//
// 	if (x1 >= x2 || y1 >= y2) return;
//
// 	int fill_width = x2 - x1;
// 	int fill_height = y2 - y1;
//
// 	UINT32 *dest = &render_target[y1 * ppsl + x1];
//
// 	for (int i = 0; i < fill_height; i++)
// 	{
// 		for (int j = 0; j < fill_width; j++)
// 		{
// 			dest[j] = color;
// 		}
//
// 		dest += ppsl;
// 	}
// }
// void draw_rect(int x, int y, UINT32 w, UINT32 h, UINT32 color)
// {
// 	int x1 = (x < 0) ? 0 : x;
// 	int y1 = (y < 0) ? 0 : y;
// 	int x2 = (x + (int)w > (int)HResolution) ? (int)HResolution : (x + (int)w);
// 	int y2 = (y + (int)h > (int)VResolution) ? (int)VResolution : (y + (int)h);
//
// 	if (x1 >= x2 || y1 >= y2) return;
//
// 	int fill_width = x2 - x1;
// 	int fill_height = y2 - y1;
//
// 	UINT32 *dest = &render_target[y1 * ppsl + x1];
//
// 	for (int i = 0; i < fill_height; i++)
// 	{
// 		if (fill_width >= 2)
// 		{
// 			sys_memset64(dest, color, fill_width >> 1);
// 		}
//
// 		if (fill_width & 1)
// 		{
// 			dest[fill_width - 1] = color;
// 		}
//
// 		dest += ppsl;
// 	}
// }
void draw_rect(int x, int y, UINT32 w, UINT32 h, UINT32 color)
{
	int x1 = (x < 0) ? 0 : x;
	int y1 = (y < 0) ? 0 : y;
	int x2 = (x + (int)w > (int)HResolution) ? (int)HResolution : (x + (int)w);
	int y2 = (y + (int)h > (int)VResolution) ? (int)VResolution : (y + (int)h);

	if (x1 >= x2 || y1 >= y2) return;

	int fill_width = x2 - x1;
	int fill_height = y2 - y1;

	UINT32 *dest = &render_target[y1 * ppsl + x1];

	for (int i = 0; i < fill_height; i++)
	{
		sys_memset32(dest, color, fill_width);
		dest += ppsl;
	}
	mark_dirty(x1, y1, fill_width, fill_height);
}
