#pragma once

#include <stdint.h>

struct inode {
    uint32_t blocks[12];
    uint32_t size;
    int permission; //1 is r, 0 is w
};

struct directory {
    char name[32];
    struct inode inode;
};

struct superblock {
    struct directory* directories;
    uint32_t* disk_bitmap;
    uint32_t total_disk_size;
    uint32_t num_inodes;
};

/*
Superblock information:
- needs to track both disk space and inodes
variables:
- array start for directories
- disk bitmap
- disk total size
- stack current size

Inode Information:
- tracks disk blocks using an array
- tracks size of disk allocation
- less important:
    - ownership
    - r/w
    - folders? file structure?

General Layout:
Supernode -> directory array -> inode -> actual disk block
bitmap tracks disk availability
*/

void mem_cpy(uint32_t* src, uint32_t* dest, uint32_t size);
void fs_init();
