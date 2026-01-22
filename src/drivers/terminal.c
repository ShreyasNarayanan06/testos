#include <stddef.h>
#include <stdint.h>
#include "../../headers/io.h"
#include "../../headers/terminal.h"
#include "../../headers/commands.h"
#include "../../headers/buffer.h"
#include "../../headers/textedit.h"
#include "../../headers/keyboard.h"

#define PROMPT "naray % "
#define PROMPT_LENGTH 8


static inline uint8_t vga_entry_color(enum vga_color foreground, enum vga_color background) {
	return foreground | background << 4; //combines the foreground and background colors and puts them into 8 bytes which is the hardware standard for the VGA Color Table
}

static inline uint16_t vga_entry(unsigned char character, uint8_t color) {
	return (uint16_t) character | (uint16_t) color << 8; //combines the character and color into 16 bytes which is the hardware standard for the VGA Entry Table
}

size_t strlen(const char* str) { //returns the length of a string
	size_t len = 0;
	while (str[len]) {
		len++;
	}
	return len;
}

#define VGA_WIDTH 80 //sets the width of the VGA buffer to 80
#define VGA_HEIGHT 25 //sets the height of the VGA buffer to 25
#define VGA_MEMORY 0xB8000 //sets the VGA memory to the address 0xB8000

size_t t_row;
size_t t_col;
uint8_t t_color;
uint16_t* t_buffer = (uint16_t*)VGA_MEMORY;;

void update_cursor(int x, int y) {
    uint16_t pos = y * VGA_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_initialize(void) { //initializes the terminal
	t_row = 0;
	t_col = 0;
	t_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

	for (size_t y = 0; y < VGA_HEIGHT; y++) { //clears the terminal
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t location = y * VGA_WIDTH + x;
			t_buffer[location] = vga_entry(' ', t_color);
		}
	}

	terminal_writestring(PROMPT);
	update_cursor(t_col, t_row);
}

void terminal_setcolor(uint8_t color) { //sets the color of the terminal
	t_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) { //puts a character at a specific location
	const size_t location = y * VGA_WIDTH + x;
	t_buffer[location] = vga_entry(c, color);
}

void terminal_putchar(char c) { //puts a character at the current location
	if (c == '\n') {
        terminal_enter();
        return;
    }

	terminal_putentryat(c, t_color, t_col, t_row);
	if (++t_col == VGA_WIDTH) {
		t_col = 0;
		if (++t_row == VGA_HEIGHT)
			t_row = 0;
	}

	update_cursor(t_col, t_row);
}

void terminal_write(const char* data, size_t size) {
    for(size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void terminal_writenumber(uint32_t n) {
    if (n == 0) {
        terminal_putchar('0');
        return;
    }

    char buf[11]; 
    int i = 10;
    buf[i] = '\0';

    while (n > 0) {
        buf[--i] = (n % 10) + '0';
        n /= 10;
    }

    terminal_writestring(&buf[i]);
}

void terminal_removechar() {
	size_t limit = (curr_buffer == TEXT_BUFFER) ? 0: PROMPT_LENGTH;
	
	if (t_col > limit) {
        t_col--;
    } else {
		return;
	}
    
	terminal_putentryat(' ', t_color, t_col, t_row);
	command_buffer_removechar(buffer_size, buffer);
	update_cursor(t_col, t_row);
}

void terminal_enter() {
	if(t_row == VGA_HEIGHT - 1) t_row = 0;
	else t_row++;

	t_col = 0;
	
	clear_buffer(&curr_size, command_buffer);
	terminal_writestring(PROMPT);
	update_cursor(t_col, t_row);
}

void terminal_enter_no_prompt() {
	if(t_row == VGA_HEIGHT - 1) t_row = 0;
	else t_row++;

	t_col = 0;
	
	update_cursor(t_col, t_row);
}

void terminal_arrow_right() {
	if(t_col == VGA_WIDTH - 1) t_col = 0;
	else t_col++;

	update_cursor(t_col, t_row);
}

void terminal_arrow_left() {
	if(t_col == 0) t_col = VGA_WIDTH - 1;
	else t_col--;

	update_cursor(t_col, t_row);

}

void terminal_arrow_up() {
	if(t_row == 0) t_row = VGA_HEIGHT - 1;
	else t_row--;

	update_cursor(t_col, t_row);
}

void terminal_arrow_down() {
	if(t_row == VGA_HEIGHT - 1) t_row = 0;
	else t_row++;

	update_cursor(t_col, t_row);
}

void terminal_clear() {
    for (size_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        t_buffer[i] = vga_entry(' ', t_color);
    }

    t_row = 0;
    t_col = 0;

    terminal_writestring(PROMPT);
    update_cursor(t_col, t_row);
}

void terminal_clear_TE() {
	for (size_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        t_buffer[i] = vga_entry(' ', t_color);
    }

    t_row = 0;
    t_col = 0;

    update_cursor(t_col, t_row);
}

void terminal_set_background(uint8_t bg_color) {
    uint8_t current_fg = t_color & 0x0F;
    t_color = current_fg | (bg_color << 4);

    for (size_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        uint8_t c = (uint8_t)(t_buffer[i] & 0xFF); 
        t_buffer[i] = vga_entry(c, t_color);       
    }
}

void terminal_set_cursor(int x, int y) {
    if (x >= VGA_WIDTH) x = VGA_WIDTH - 1;
    if (y >= VGA_HEIGHT) y = VGA_HEIGHT - 1;

    t_col = x;
    t_row = y;
    update_cursor(t_col, t_row);
}