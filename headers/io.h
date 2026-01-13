#pragma once
// env error counter = 3

#include "VMM.h"
#include "fs.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
  asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static inline uint16_t inw(uint16_t port) { 
    uint16_t ret; 
    asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port)); 
    return ret; 
}

// getting information from the disk

static inline uint32_t* ATA_identify() {

  // write 0xA0 to I/O 0x1F6 to select the primary device
  outb(0x1F6, 0xA0);

  // reset all of the LBAs to 0 so we can get the information regarding the
  // whole disk
  outb(0x1F2, 0x0);
  outb(0x1F3, 0x0);
  outb(0x1F4, 0x0);
  outb(0x1F5, 0x0);

  // write the command code 0xEC to the I/O port 0x1F7
  outb(0x1F7, 0xEC);

  while(inb(0x1F7) & 0x80);
  while(!(inb(0x1F7) & 0x08));

  uint16_t* identify_addr = (uint16_t*)kmalloc(512);
  if(!identify_addr) return NULL;

  for(int i = 0; i < 256; i++) {
    identify_addr[i] = inw(0x1F0);
  }

  return (uint32_t*)identify_addr;
}