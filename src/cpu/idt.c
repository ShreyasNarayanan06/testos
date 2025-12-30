#include "../../headers/idt.h"
#include <stdint.h>

extern void load_idt(uint32_t idtp);
extern void IRQ1_handler();

struct IDT idt[256]; //IDT struct array
struct IDT_Ptr idtp;

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

