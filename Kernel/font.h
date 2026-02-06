#ifndef _FONT_H_
#define _FONT_H_

#include "../Inc/type.h"

#define PSF2_MAGIC 0x864ab572

#pragma pack(1)
typedef struct
{
	UINT32 magic;
	UINT32 version;
	UINT32 headersize;
	UINT32 flags;
	UINT32 num_glyphs;
	UINT32 bytes_per_glyph;
	UINT32 height;
	UINT32 width;
} psf2_header_t;
#pragma pack()

extern UINT32 font_width;
extern UINT32 font_height;

void font_init(unsigned char *psf_data);
void put_char(char c, int x, int y, UINT32 color);

// ter-v32b.psf but only ascii
extern unsigned char ter_ascii_psf[];
extern unsigned int ter_ascii_psf_len;
#endif // _FONT_H_
