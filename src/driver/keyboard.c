#include <stdint.h>
#include "../../headers/io.h"
#include "../../headers/keyboard.h"

int shift_pressed;
int is_extended;

void reboot() {
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64); // Wait for controller to be ready
    outb(0x64, 0xFE);     // Send the "Pulse Reset Pin" command
}

void process_keyboard_input(uint8_t scancode){

	if(scancode == 0x2A || scancode == 0x36 || ((scancode == 0x3A) && !shift_pressed)) {
		shift_pressed = 1;
		return;
	}
	else if(scancode == 0xAA || scancode == 0xB6 || scancode == 0xB7 || ((scancode == 0x3A) && shift_pressed)) {
		shift_pressed = 0;
		return;
	}

	
	if(scancode & 0x80) return;

	switch(scancode) {
		// Numbers
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

		// Letters
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
		case 0x1E: terminal_putchar(shift_pressed ? 'A' : 'a'); break;
		case 0x1F: terminal_putchar(shift_pressed ? 'S' : 's'); break;
		case 0x20: terminal_putchar(shift_pressed ? 'D' : 'd'); break;
		case 0x21: terminal_putchar(shift_pressed ? 'F' : 'f'); break;
		case 0x22: terminal_putchar(shift_pressed ? 'G' : 'g'); break;
		case 0x23: terminal_putchar(shift_pressed ? 'H' : 'h'); break;
		case 0x24: terminal_putchar(shift_pressed ? 'J' : 'j'); break;
		case 0x25: terminal_putchar(shift_pressed ? 'K' : 'k'); break;
		case 0x26: terminal_putchar(shift_pressed ? 'L' : 'l'); break;
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
		case 0x0E: is_extended = 1; terminal_removechar(); break; // Backspace
		case 0x0F: terminal_writestring("   "); break; // Tab

		// Arrow Keys
		case 0x48: terminal_arrow_up(); break;
		case 0x50: terminal_arrow_down(); break;
		case 0x4B: terminal_arrow_left(); break;
		case 0x4D: terminal_arrow_right(); break;
		

		//Random other characters
		case 0x33: terminal_putchar(shift_pressed ? '<' : ','); break;
		case 0x34: terminal_putchar(shift_pressed ? '>' : '.'); break;
		case 0x35: terminal_putchar(shift_pressed ? '?' : '/'); break;
		case 0x4A: terminal_putchar(shift_pressed ? '_' : '-'); break;
		case 0x0D: terminal_putchar(shift_pressed ? '+' : '='); break;
		case 0x54: terminal_putchar(shift_pressed ? '{' : '['); break;
		case 0x1B: terminal_putchar(shift_pressed ? '}' : ']'); break;
		case 0x2B: terminal_putchar(shift_pressed ? '|' : '\\'); break;
		case 0x4C: terminal_putchar(shift_pressed ? ':' : ';'); break;
		case 0x52: terminal_putchar(shift_pressed ? '"' : '\''); break;
		case 0x29: terminal_putchar(shift_pressed ? '~' : '`'); break;

		case 0x01: reboot(); break; //Reboot
	}	
}

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);
	process_keyboard_input(scancode);
	outb(0x20, 0x20);
}

