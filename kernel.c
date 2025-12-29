//env error counter = 1
//Need to figure out the whole IDT ptr and PIC thing

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "io.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

//Global variables
int shift_pressed = 0;

struct IDT {
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  zero;            // unused, set to 0
   uint8_t  type_attributes; // gate type, dpl, and p fields
   uint16_t offset_2;        // offset bits 16..31
};

struct IDT_Ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)); //__attribute__((packed)) is used to prevent the compiler from adding padding to the struct

struct IDT idt[256]; //IDT struct array
struct IDT_Ptr idtp;

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
uint16_t* t_buffer = (uint16_t*) VGA_MEMORY;

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
}

void terminal_setcolor(uint8_t color) { //sets the color of the terminal
	t_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) { //puts a character at a specific location
	const size_t location = y * VGA_WIDTH + x;
	t_buffer[location] = vga_entry(c, color);
}

void terminal_putchar(char c) { //puts a character at the current location
	terminal_putentryat(c, t_color, t_col, t_row);
	if (++t_col == VGA_WIDTH) {
		t_col = 0;
		if (++t_row == VGA_HEIGHT)
			t_row = 0;
	}
}

void terminal_write(const char* data, size_t size) {
    for(size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void terminal_removechar() {
	if (t_col > 0) {
        t_col--;
    } else if (t_row > 0) {
        t_row--;
        t_col = VGA_WIDTH - 1;
    } else {
        return; 
    }

	terminal_putentryat(' ', t_color, t_col, t_row);
}

void terminal_enter() {
	if(t_row == VGA_HEIGHT - 1) return;
	t_row++;
	t_col = 0;
}

void pic_init() {
    // ICW1: Start initialization in cascade mode
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2: Vector offsets (Master = 32, Slave = 40)
    outb(0x21, 0x20); // Keyboard (IRQ1) becomes 33
    outb(0xA1, 0x28);

    // ICW3: Tell Master that Slave is at IRQ2 (0000 0100)
    outb(0x21, 0x04);
    // ICW3: Tell Slave its cascade identity (0000 0010)
    outb(0xA1, 0x02);

    // ICW4: Put PICs in 8086/88 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Mask: 0x00 enables all IRQs
    outb(0x21, 0xFD); 
    outb(0xA1, 0xFF); // Disable all Slave IRQs
}

void reboot() {
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64); // Wait for controller to be ready
    outb(0x64, 0xFE);     // Send the "Pulse Reset Pin" command
}

void process_keyboard_input(uint8_t scancode){

	if(scancode == 0x2A || scancode == 0x36) {
		shift_pressed = 1;
		return;
	}
	else if(scancode == 0xAA || scancode == 0xB6) {
		shift_pressed = 0;
		return;
	}

	
	if(scancode & 0x80) return;

	switch(scancode) {
		// Numbers (Top Row)
		case 0x02: terminal_putchar(shift_pressed ? '!' : '1'); break;
		case 0x03: terminal_putchar(shift_pressed ? '@' : '2'); break;
		case 0x04: terminal_putchar(shift_pressed ? '#' : '3'); break;
		case 0x05: terminal_putchar(shift_pressed ? '$' : '4'); break;
		case 0x06: terminal_putchar(shift_pressed ? '%' : '5'); break;
		case 0x07: terminal_putchar(shift_pressed ? '^' : '6'); break;
		case 0x08: terminal_putchar(shift_pressed ? '&' : '7'); break;
		case 0x09: terminal_putchar(shift_pressed ? '*' : '8'); break;
		case 0x0A: terminal_putchar(shift_pressed ? '(' : '9'); break;
		case 0x0B: terminal_putchar(shift_pressed ? ')' : '0'); break;

		// Letters (Row 1)
		case 0x10: terminal_putchar(shift_pressed ? 'Q' : 'q'); break;
		case 0x11: terminal_putchar(shift_pressed ? 'W' : 'w'); break;
		case 0x12: terminal_putchar(shift_pressed ? 'E' : 'e'); break;
		case 0x13: terminal_putchar(shift_pressed ? 'R' : 'r'); break;
		case 0x14: terminal_putchar(shift_pressed ? 'T' : 't'); break;
		case 0x15: terminal_putchar(shift_pressed ? 'Y' : 'y'); break;
		case 0x16: terminal_putchar(shift_pressed ? 'U' : 'u'); break;
		case 0x17: terminal_putchar(shift_pressed ? 'I' : 'i'); break;
		case 0x18: terminal_putchar(shift_pressed ? 'O' : 'o'); break;
		case 0x19: terminal_putchar(shift_pressed ? 'P' : 'p'); break;

		// Letters (Row 2)
		case 0x1E: terminal_putchar(shift_pressed ? 'A' : 'a'); break;
		case 0x1F: terminal_putchar(shift_pressed ? 'S' : 's'); break;
		case 0x20: terminal_putchar(shift_pressed ? 'D' : 'd'); break;
		case 0x21: terminal_putchar(shift_pressed ? 'F' : 'f'); break;
		case 0x22: terminal_putchar(shift_pressed ? 'G' : 'g'); break;
		case 0x23: terminal_putchar(shift_pressed ? 'H' : 'h'); break;
		case 0x24: terminal_putchar(shift_pressed ? 'J' : 'j'); break;
		case 0x25: terminal_putchar(shift_pressed ? 'K' : 'k'); break;
		case 0x26: terminal_putchar(shift_pressed ? 'L' : 'l'); break;

		// Letters (Row 3)
		case 0x2C: terminal_putchar(shift_pressed ? 'Z' : 'z'); break;
		case 0x2D: terminal_putchar(shift_pressed ? 'X' : 'x'); break;
		case 0x2E: terminal_putchar(shift_pressed ? 'C' : 'c'); break;
		case 0x2F: terminal_putchar(shift_pressed ? 'V' : 'v'); break;
		case 0x30: terminal_putchar(shift_pressed ? 'B' : 'b'); break;
		case 0x31: terminal_putchar(shift_pressed ? 'N' : 'n'); break;
		case 0x32: terminal_putchar(shift_pressed ? 'M' : 'm'); break;

		// Special
		case 0x39: terminal_putchar(' '); break; // Spacebar
		case 0x1C: terminal_enter(); break; // Enter
		case 0x0E: terminal_removechar(); break; // Backspace

		case 0x01: reboot(); break; //Reboot
	}	
}

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);
	process_keyboard_input(scancode);
	outb(0x20, 0x20);
}
extern void IRQ1_handler();
extern void load_idt(uint32_t idtp);

void init_IDT() {

	for(int i = 0; i < 256; i++) {
        idt[i].offset_1 = 0;
        idt[i].offset_2 = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
        idt[i].type_attributes = 0;
    }

	uint32_t addr = (uint32_t)IRQ1_handler;
	
	idt[33].offset_1  = addr & 0xFFFF;
    idt[33].offset_2 = (addr >> 16) & 0xFFFF;
    idt[33].selector    = 0x08; // Kernel Code Segment
	idt[33].zero = 0;
	idt[33].type_attributes = 0x8E; // 0x8E is the type attribute for an interrupt gate

	idtp.limit = (sizeof(struct IDT) * 256) - 1;
    idtp.base  = (uint32_t)&idt;

    load_idt((uint32_t)&idtp); // The ASM function
}

void kernel_main(void) {
    terminal_initialize();
	init_IDT();
	pic_init();

	asm volatile ("sti");

    while(1) {
		asm("hlt");
	}
	
}