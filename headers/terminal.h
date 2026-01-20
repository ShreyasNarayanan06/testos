#include <stdint.h>
#include <stddef.h>

extern size_t t_row;
extern size_t t_col;
extern uint8_t t_color;
extern uint16_t* t_buffer;

enum vga_color {
	VGA_COLOR_BLACK = 0, //colors are sourced from the VGA Color Table 
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_removechar();
void terminal_enter();
void terminal_arrow_right();
void terminal_arrow_left();
// void terminal_arrow_up();
// void terminal_arrow_down();
void terminal_writenumber(uint32_t n);
void terminal_clear();
void terminal_enter_no_prompt();
void terminal_scroll();
void terminal_set_background(uint8_t bg_color);

