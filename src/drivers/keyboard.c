#include <stdint.h>
#include "../../headers/io.h"
#include "../../headers/keyboard.h"
#include "../../headers/commands.h"
#include "../../headers/buffer.h"
#include "../../headers/textedit.h"

enum buffer_type curr_buffer = COMMAND_BUFFER;


int shift_pressed;
int is_extended;
int ctrl_pressed;

char* buffer;
size_t* buffer_size;

void reboot() {
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64); // Wait for controller to be ready
    outb(0x64, 0xFE);     // Send the "Pulse Reset Pin" command
}

void set_buffer() {
	if(curr_buffer == TEXT_BUFFER) {
		buffer = text_buffer;
		buffer_size = &text_buffer_size;
	}
	else if(curr_buffer == COMMAND_BUFFER) {
		buffer = command_buffer;
		buffer_size = &curr_size;
	}	
	else {
		buffer = filename_buffer;
		buffer_size = &filename_buffer_size;
	}
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

	if(scancode == 0x1D) { ctrl_pressed = 1; return; }
	if(scancode == 0x9D) { ctrl_pressed = 0; return; }

	
	if(scancode & 0x80) return;

	switch(scancode) {
		// Numbers
		case 0x02: 
				fill_buffer(shift_pressed ? '!' : '1', buffer_size, buffer);
				terminal_putchar(shift_pressed ? '!' : '1');
				break;
		case 0x03: 
				fill_buffer(shift_pressed ? '@' : '2', buffer_size, buffer);
				terminal_putchar(shift_pressed ? '@' : '2');
				break;
		case 0x04: 
				fill_buffer(shift_pressed ? '#' : '3', buffer_size, buffer);
				terminal_putchar(shift_pressed ? '#' : '3');
				break;
		case 0x05: 
				fill_buffer(shift_pressed ? '$' : '4', buffer_size, buffer);
				terminal_putchar(shift_pressed ? '$' : '4');
				break;
		case 0x06: 
				fill_buffer(shift_pressed ? '%' : '5', buffer_size, buffer);
				terminal_putchar(shift_pressed ? '%' : '5');
				break;
		case 0x07: 
				fill_buffer(shift_pressed ? '^' : '6', buffer_size, buffer);
				terminal_putchar(shift_pressed ? '^' : '6');
				break;
		case 0x08: 
				fill_buffer(shift_pressed ? '&' : '7', buffer_size, buffer);
				terminal_putchar(shift_pressed ? '&' : '7');
				break;
		case 0x09: 
				fill_buffer(shift_pressed ? '*' : '8', buffer_size, buffer);
				terminal_putchar(shift_pressed ? '*' : '8');
				break;
		case 0x0A: 
				fill_buffer(shift_pressed ? '(' : '9', buffer_size, buffer);
				terminal_putchar(shift_pressed ? '(' : '9');
				break;
		case 0x0B: terminal_putchar(shift_pressed ? ')' : '0'); break;

		// Letters
		case 0x10: 
				if(ctrl_pressed && (curr_buffer == TEXT_BUFFER)) {
					// terminal_writestring("why isnt this working?");
					close_file();
					break;
				}
				fill_buffer(shift_pressed ? 'Q' : 'q', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'Q' : 'q');
				break;
		case 0x11: 
				fill_buffer(shift_pressed ? 'W' : 'w', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'W' : 'w');
				break;
		case 0x12: 
				fill_buffer(shift_pressed ? 'E' : 'e', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'E' : 'e');
				break;
		case 0x13: 
				fill_buffer(shift_pressed ? 'R' : 'r', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'R' : 'r');
				break;
		case 0x14: 
				fill_buffer(shift_pressed ? 'T' : 't', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'T' : 't');
				break;
		case 0x15: 
				fill_buffer(shift_pressed ? 'Y' : 'y', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'Y' : 'y');
				break;
		case 0x16: 
				fill_buffer(shift_pressed ? 'U' : 'u', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'U' : 'u');
				break;
		case 0x17: 
				fill_buffer(shift_pressed ? 'I' : 'i', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'I' : 'i');
				break;
		case 0x18: 
				fill_buffer(shift_pressed ? 'O' : 'o', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'O' : 'o');
				break;
		case 0x19: 
				fill_buffer(shift_pressed ? 'P' : 'p', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'P' : 'p');
				break;
		case 0x1E: 
				fill_buffer(shift_pressed ? 'A' : 'a', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'A' : 'a');
				break;
		case 0x1F: 
				fill_buffer(shift_pressed ? 'S' : 's', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'S' : 's');
				break;
		case 0x20: 
				fill_buffer(shift_pressed ? 'D' : 'd', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'D' : 'd');
				break;
		case 0x21: 
				fill_buffer(shift_pressed ? 'F' : 'f', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'F' : 'f');
				break;
		case 0x22: 
				fill_buffer(shift_pressed ? 'G' : 'g', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'G' : 'g');
				break;
		case 0x23: 
				fill_buffer(shift_pressed ? 'H' : 'h', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'H' : 'h');
				break;
		case 0x24: 
				fill_buffer(shift_pressed ? 'J' : 'j', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'J' : 'j');
				break;
		case 0x25: 
				fill_buffer(shift_pressed ? 'K' : 'k', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'K' : 'k');
				break;
		case 0x26: 
				fill_buffer(shift_pressed ? 'L' : 'l', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'L' : 'l');
				break;
		case 0x2C: 
				fill_buffer(shift_pressed ? 'Z' : 'z', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'Z' : 'z');
				break;
		case 0x2D: 
				fill_buffer(shift_pressed ? 'X' : 'x', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'X' : 'x');
				break;
		case 0x2E: 
				fill_buffer(shift_pressed ? 'C' : 'c', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'C' : 'c');
				break;
		case 0x2F: 
				fill_buffer(shift_pressed ? 'V' : 'v', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'V' : 'v');
				break;
		case 0x30: 
				fill_buffer(shift_pressed ? 'B' : 'b', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'B' : 'b');
				break;
		case 0x31: 
				fill_buffer(shift_pressed ? 'N' : 'n', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'N' : 'n');
				break;
		case 0x32: 
				fill_buffer(shift_pressed ? 'M' : 'm', buffer_size, buffer);
				terminal_putchar(shift_pressed ? 'M' : 'm');
				break;

		// Special
		case 0x39: 
			fill_buffer(' ', buffer_size, buffer);
			terminal_putchar(' '); 
			break; // Spacebar
		case 0x1C: //enter
			if(curr_buffer == TEXT_BUFFER) {
				terminal_enter_no_prompt();
			} else if(curr_buffer == FILENAME_BUFFER) {
				curr_buffer = COMMAND_BUFFER;
				return_to_terminal();
			} else {
				check_command();
				clear_buffer(buffer_size, buffer);
			}
			break; 
		
		case 0x0E: is_extended = 1; terminal_removechar(); break; // Backspace
		case 0x0F: terminal_writestring("   "); break; // Tab

		// Arrow Keys
		case 0x48: if(curr_buffer == TEXT_BUFFER) terminal_arrow_up(); break;
		case 0x50: if(curr_buffer == TEXT_BUFFER) terminal_arrow_down(); break;
		case 0x4B: terminal_arrow_left(); break;
		case 0x4D: terminal_arrow_right(); break;
		

		//Random other characters
		case 0x33: 
					fill_buffer(shift_pressed ? '<' : ',', buffer_size, buffer);
					terminal_putchar(shift_pressed ? '<' : ',');
					break;
		case 0x34: 
					fill_buffer(shift_pressed ? '>' : '.', buffer_size, buffer);
					terminal_putchar(shift_pressed ? '>' : '.');
					break;
		case 0x35: 
					fill_buffer(shift_pressed ? '?' : '/', buffer_size, buffer);
					terminal_putchar(shift_pressed ? '?' : '/');
					break;
		case 0x4A: 
					fill_buffer(shift_pressed ? '_' : '-', buffer_size, buffer);
					terminal_putchar(shift_pressed ? '_' : '-');
					break;
		case 0x0D: 
					fill_buffer(shift_pressed ? '+' : '=', buffer_size, buffer);
					terminal_putchar(shift_pressed ? '+' : '=');
					break;
		case 0x54: 
					fill_buffer(shift_pressed ? '{' : '[', buffer_size, buffer);
					terminal_putchar(shift_pressed ? '{' : '[');
					break;
		case 0x1B: 	
					fill_buffer(shift_pressed ? '}' : ']', buffer_size, buffer);
					terminal_putchar(shift_pressed ? '}' : ']');
					break;
		case 0x2B: 	
					fill_buffer(shift_pressed ? '|' : '\\', buffer_size, buffer);
					terminal_putchar(shift_pressed ? '|' : '\\');
					break;
		case 0x4C: 	
					fill_buffer(shift_pressed ? ':' : ';', buffer_size, buffer);
					terminal_putchar(shift_pressed ? ':' : ';');
					break;
		case 0x52: 	
					fill_buffer(shift_pressed ? '"' : '\'', buffer_size, buffer);
					terminal_putchar(shift_pressed ? '"' : '\'');
					break;
		case 0x29: 	
					fill_buffer(shift_pressed ? '~' : '`', buffer_size, buffer);
					terminal_putchar(shift_pressed ? '~' : '`');
					break;

		case 0x01: reboot(); break; //Reboot
	}	
}

void keyboard_handler(void) {
	set_buffer();
    uint8_t scancode = inb(0x60);
	process_keyboard_input(scancode);
	outb(0x20, 0x20);
}

