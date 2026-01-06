#include <stdint.h>
#include "../../headers/io.h"
#include "../../headers/terminal.h"
#include "../../headers/PMM.h"

uint32_t map_length = 0;
uint32_t map_addr = 0;
uint32_t num_pages = 0;
uint32_t* page_bitmap = 0;


void MIS_data(uint32_t mbptr) {
    uint32_t size = *(uint32_t*)(mbptr + 44);
    terminal_writestring("Multiboot size: ");
    terminal_writenumber(size);
    terminal_enter();

    uint32_t addr = *(uint32_t*)(mbptr + 48);
    terminal_writestring("Multiboot addr: ");
    terminal_writenumber(addr);
    terminal_enter();
    
}

void PMM_init(uint32_t mbptr) {
    map_length = *(uint32_t*)(mbptr + 44); //the mbptr points to the beginning of the MIS and at offset 44 is the length of the MIS
    map_addr = *(uint32_t*)(mbptr + 48); //at offset 48 is the address of the memory map

    uint32_t offset = 0;
    uint64_t max_addr = 0;
    
    while(offset < map_length) { //This while loop goes through the memory map and adds up the total free memory so we have a total
        struct memory_map_entry* entry = (struct memory_map_entry*)(map_addr + offset);
        if(entry->type == 1) max_addr = entry->base_addr + entry->length;

        offset += entry->size + 4;
    }

    num_pages = (uint32_t)(max_addr / 4096); //max_addr is in bytes so we divide by 4096 to get the number of pages
    page_bitmap = (uint32_t*)0x200000; //page_bitmap is the bitmap which says if a page is free or not, we set it to some far off memory location

    for(uint32_t i = 0; i < num_pages/32; i++) {
        page_bitmap[i] = 1; //set all of the bits in the page bitmap to 1 which is reserved.
    }

    //we are now going to fill in whether or not the pages are free
    offset = 0;
    while (offset < map_length) {
        struct memory_map_entry* entry = (struct memory_map_entry*)(map_addr + offset);

        if (entry->type == 1) {
            uint64_t start = entry->base_addr;
            uint64_t end = start + entry->length;

            // Align start up and end down to 4096
            uint32_t start_page = (start + 4095) / 4096;
            uint32_t end_page = end / 4096;

            for (uint32_t p = start_page; p < end_page; p++) {
                page_bitmap[p / 32] &= ~(1 << (p % 32)); //set the bit accordingly
                // terminal_writenumber(page_bitmap[p / 32]);
                // terminal_writestring(" ");
                
            }
        }
        offset += entry->size + 4;
    }
}

uint32_t pm_alloc_page() {
    for(uint32_t i = 0; i < num_pages/32; i++) { // /32 because each uint_32 can hold 32 bits
        if(page_bitmap[i] != 0xFFFFFFFF) {
            for(uint32_t j = 0; j < 32; j++) {
                if(page_bitmap[i] & (1 >> j)) { //the & operation checks what an individual bit is set to. It uses right shift
                    page_bitmap[i] |= (1 >> j);
                    return (i * 32 + j) * 4096; //return the uint_32 that its in + the index of the bit in the uint_32 and * 4096 to get to an actual addr
                }
            }
        }
    }
    return 0;
}

