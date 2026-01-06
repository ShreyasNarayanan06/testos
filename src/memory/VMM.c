#include "../../headers/terminal.h"
#include "../../headers/PMM.h"
#include "../../headers/VMM.h"
#include <stdint.h>

extern void set_page_directory(uint32_t);
extern uint32_t get_page_directory();
extern void enable_paging();
extern uint32_t read_faulting_address();
extern void flush_TLB(uint32_t);
uint32_t* page_directory;
uint32_t* next_free;
uint32_t* heap_start;
uint32_t* heap_end;
struct heap_header* prev_free = NULL;

#define virtual_PD ((uint32_t*)0xFFFFF000)
#define virtual_PTs(pdidx) ((uint32_t*)(0xFFC00000 + (pdidx * 4096)))


void vmem_info() {
    terminal_writestring("Top level page table: ");
    terminal_writenumber(get_page_directory());
    terminal_enter();
}

void vmem_init() {
    uint32_t page_directory_addr = (uint32_t)pm_alloc_page();
    page_directory = (uint32_t*)page_directory_addr;

    //set all of the bits in the top level page table to 0
    for(int i = 0; i < 1024; i++) page_directory[i] = 0;

    uint32_t page_table_1_addr = (uint32_t)pm_alloc_page();
    uint32_t* page_table_1 = (uint32_t*)page_table_1_addr;

    for(int i = 0; i < 1024; i++) page_table_1[i] = 0; //same as before

    page_directory[0] = (page_table_1_addr | 0x3); //set the page directory index 0 to the pt1 we made

    page_directory[1023] = (uint32_t)page_directory | 0x3;

    set_page_directory(page_directory_addr); //set the page directory

    enable_paging(); //enable paging
}

void heap_init() {
    heap_start = (uint32_t*)0xD0000000;
    heap_end = (uint32_t*)0xE0000000;
    next_free = heap_start;
}

uint32_t* kmalloc(uint32_t size) { //fix approach later. needs to not be a bump allocator
    struct heap_header* header = (struct heap_header*)next_free;
    header->is_free = 0;
    header->size = size;
    header->prev = prev_free;
    header->next = NULL;
    prev_free = header;
    
    uint32_t addr = (uint32_t)next_free + sizeof(struct heap_header) + size;

    if (addr % 8 != 0) {
        addr += (8 - (addr % 8));
    }

    next_free = (uint32_t*)addr;

    if (next_free > heap_end) {
        terminal_writestring("Out of memory\n");
        while(1);
    }

    return (uint32_t*)(header + 1);
}

void fault_handler() {
    uint32_t fault_addr = read_faulting_address();

    uint32_t pdidx = fault_addr >> 22;
    uint32_t ptidx = (fault_addr >> 12) & 0x3FF;

    if((virtual_PD[pdidx] & 0x1) == 0) {
        virtual_PD[pdidx] = pm_alloc_page() | 0x3;

        for(int i = 0; i < 1024; i++) virtual_PTs(pdidx)[i] = 0;
    }

    
    if((virtual_PTs(pdidx)[ptidx] & 0x1) == 0) {
        virtual_PTs(pdidx)[ptidx] = pm_alloc_page() | 0x3;
    }

    flush_TLB(fault_addr);
    asm volatile ("iret");
}

void kfree(uint32_t* data_ptr) {
    struct heap_header* header = (struct heap_header*)data_ptr - 1;
    header->is_free = 1;

    if(header->next != NULL && header->next->is_free) {
        header->size += header->next->size + sizeof(struct heap_header);
        header->next = header->next->next;
        if(header->next) header->next->prev = header;
    }
    
}