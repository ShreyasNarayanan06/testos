#include "../../../headers/PMM.h"
#include "../../../headers/io.h"
#include "../../../headers/terminal.h"
#include "../../../headers/fs.h"
#include <stdint.h>
#define disk_sector_size 512


void mem_cpy(uint32_t* src, uint32_t* dest, uint32_t size) { //dunno if this is the correct file to have mem_cpy in
    for(uint32_t i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

/*
Work to do for file management
1. Gather data from the Disk using ATA Identify
2. initialize the structure of the file system
3. make write/read disk commands
4. write the structure to the disk
5. create files do io etc...
*/

void fs_init() {
    uint32_t* identify_addr = ATA_identify();

    if (identify_addr == 0) {
        terminal_writestring("Error: Disk not found or error.\n");
        return;
    }
    
    uint16_t* buffer = (uint16_t*)identify_addr;
    uint32_t sector_count = (uint32_t)buffer[60] | ((uint32_t)buffer[61] << 16);

    // terminal_writestring("Sector Count: ");
    // terminal_writenumber(sector_count);
    // terminal_writestring("\n");

    struct superblock* sb = (struct superblock*)kmalloc(sizeof(struct superblock));
    



}