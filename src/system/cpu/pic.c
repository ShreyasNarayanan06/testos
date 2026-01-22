#include "../../headers/pic.h"
#include "../../headers/io.h"

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