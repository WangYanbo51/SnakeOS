#include "../Inc/boot.h"
#include "keyboard.h"
#include "font.h"
#include "draw.h"
// #include "utils.h"

__attribute__((section(".text.entry")))
__attribute__((sysv_abi))
UINT64 kernel_main(BOOT_CONFIG *BootConfig)
{
	UINT32 Blue           = 0xFF18679A;
	UINT32 Orange         = 0xFFFF7F50;
	draw_init(BootConfig->VideoConfig);
	font_init(ter_ascii_psf);

	while (1)
	{
		fill_screen(Blue);
		put_char('A', 20, 20, Orange);
		printf_at(1200, 700, Orange, "Test: %x", 0x1234F);
    draw_rect(100, 100, 100, 100, Orange);
    show_frame();
		get_kb();

    fill_screen(Orange);
		put_char('B', 20, 20, Blue);
		printf_at(1200, 700, Blue, "Test: %x", 0xABCDE);
    draw_rect(100, 100, 100, 100, Blue);
    show_frame();
		get_kb();
	}

	return 0x1234;
}
