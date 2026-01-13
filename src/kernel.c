//env error counter = 1
//Need to figure out the whole IDT ptr and PIC thing

#include "../headers/terminal.h"
#include "../headers/keyboard.h"
#include "../headers/pic.h"
#include "../headers/idt.h"
#include "../headers/PMM.h"
#include "../headers/VMM.h"
#include "../headers/fs.h"

void kernel_main(uint32_t mbptr) {
    terminal_initialize();
	init_IDT();
	pic_init();
	PMM_init(mbptr);
    heap_init();
	vmem_init();
	fs_init();

	asm volatile ("sti");

    while(1) {
		asm("hlt");
	}
	
}