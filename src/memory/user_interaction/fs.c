#include "../../../headers/fs.h"
#include "../../../headers/PMM.h"

#define disk_sector_size 512

void mem_cpy(uint8_t* src, uint8_t* dest, uint32_t size) { //dunno if this is the correct file to have mem_cpy in
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
