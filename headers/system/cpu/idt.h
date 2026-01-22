#include <stdint.h>

struct IDT {
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  zero;            // unused, set to 0
   uint8_t  type_attributes; // gate type, dpl, and p fields
   uint16_t offset_2;        // offset bits 16..31
} __attribute__((packed));

extern struct IDT idt[256]; //IDT struct array
extern struct IDT_Ptr idtp;

struct IDT_Ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)); //__attribute__((packed)) is used to prevent the compiler from adding padding to the struct

void init_IDT();
