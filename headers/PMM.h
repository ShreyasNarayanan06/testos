#include <stdint.h>

struct memory_map_entry {
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
} __attribute__((packed));

extern uint32_t map_length;
extern uint32_t map_addr;
extern uint32_t num_pages;
extern uint32_t* page_bitmap;

void MIS_data(uint32_t mbptr);
void PMM_init(uint32_t mbptr);
uint32_t pm_alloc_page();