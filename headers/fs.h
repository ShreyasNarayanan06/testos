#pragma once

#include <stdint.h>

struct inode {
    uint32_t blocks[12];
    uint32_t size;
    int type; //1 is file, 0 is directory
    int permission; //1 is r/w, 0 is read only
};

struct directory {
    char name[32];
    struct inode inode;
};

struct superblock {
    uint32_t directories_LBA;
    uint32_t disk_bitmap_LBA;
    uint32_t total_disk_size;
    uint32_t num_inodes;
    uint32_t magic_number;
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

extern struct superblock* sb;
extern uint32_t* disk_bitmap;
extern struct directory* directories;

void mem_cpy(uint32_t* src, uint32_t* dest, uint32_t size);
uint16_t* disk_read(uint32_t LBA, uint32_t sector_count);
void disk_write(uint32_t LBA, uint32_t sector_count, uint16_t* buffer);
void format_disk();
void fs_init();
