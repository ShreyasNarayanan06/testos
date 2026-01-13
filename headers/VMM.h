#pragma once
#include <stdint.h>

void vmem_info();
void verify_cr3_address(uint32_t phys_addr);
void vmem_init();
void heap_init();
uint32_t *kmalloc(uint32_t size);
void fault_handler();
void kfree(uint32_t *data_ptr);

struct heap_header {
  uint32_t size;
  struct heap_header *next;
  struct heap_header *prev;
  int is_free;
};
