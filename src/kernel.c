//env error counter = 1
//Need to figure out the whole IDT ptr and PIC thing

#include "../headers/terminal.h"
#include "../headers/keyboard.h"
#include "../headers/pic.h"
#include "../headers/idt.h"

void kernel_main(void) {
    terminal_initialize();
	init_IDT();
	pic_init();

	asm volatile ("sti");

    while(1) {
		asm("hlt");
	}
	
}