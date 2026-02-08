#include "../Inc/boot.h"
#include "keyboard.h"
#include "font.h"
#include "draw.h"
#include "time.h"
// #include "utils.h"

__attribute__((section(".text.entry")))
__attribute__((sysv_abi))
UINT64 kernel_main(BOOT_CONFIG *BootConfig)
{
	UINT32 Blue           = 0xFF18679A;
	UINT32 Orange         = 0xFFFF7F50;
	draw_init(BootConfig->VideoConfig);
	font_init(ter_ascii_psf);

	// while (1)
	// {
	// 	fill_screen(Blue);
	// 	put_char('A', 20, 20, Orange);
	// 	printf_at(1200, 700, Orange, "Test: %x", 0x1234F);
	//    draw_rect(100, 100, 100, 100, Orange);
	//    show_frame();
	// 	get_kb();
	//
	//    fill_screen(Orange);
	// 	put_char('B', 20, 20, Blue);
	// 	printf_at(1200, 700, Blue, "Test: %x", 0xABCDE);
	//    draw_rect(100, 100, 100, 100, Blue);
	//    show_frame();
	// 	get_kb();
	// }
	fill_screen(Blue);
	int old_rect_x = 0, old_rect_y = 0;
	int rect_x = 0, rect_y = 0;
	// int px = 1, py = 1;
	// while (1)
	// {
	// 	draw_rect(old_rect_x, old_rect_y, 100, 100, Blue);
	//    old_rect_x = rect_x;
	//    old_rect_y = rect_y;
	// 	draw_rect(rect_x, rect_y, 100, 100, Orange);
	//    printf_at(rect_x + 10, rect_y + 25, Blue, "Snake");
	//    show_frame();
	//    if ()
	//    rect_x += px, rect_y += py;
	//    if (rect_x < 0 || rect_x + 100 >= (int)HResolution)
	//      px = -px;
	//    if (rect_y < 0 || rect_y + 100 >= (int)VResolution)
	//      py = -py;
	//    delay_cycles(30000000);
	// }
	while (1)
	{
		draw_rect(old_rect_x, old_rect_y, 100, 100, Blue);
		old_rect_x = rect_x;
		old_rect_y = rect_y;
		KEY_EVENT event = get_kb_event();
		switch (event)
		{
		case KEY_UP:
			rect_y -= 10;
			break;
		case KEY_DOWN:
			rect_y += 10;
			break;
		case KEY_LEFT:
			rect_x -= 10;
			break;
		case KEY_RIGHT:
			rect_x += 10;
			break;
		default:
			break;
		}
		draw_rect(rect_x, rect_y, 100, 100, Orange);
    show_frame();
    delay_cycles(30000000);
	}
	return 0;
}
