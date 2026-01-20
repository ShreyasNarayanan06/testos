#include "../../../headers/fs.h"
#include "../../../headers/PMM.h"
#include "../../../headers/io.h"
#include "../../../headers/string.h"
#include "../../../headers/terminal.h"
#include <stdint.h>
#define disk_sector_size 512

struct superblock *sb;
uint32_t *disk_bitmap;
struct directory *directories;

void ATA_init() {
  // Software reset of ATA controller
  outb(0x3F6, 0x04); // Set SRST bit
  for (int i = 0; i < 4; i++)
    inb(0x3F6);      // Wait ~400ns
  outb(0x3F6, 0x00); // Clear SRST bit
  for (int i = 0; i < 4; i++)
    inb(0x3F6); // Wait ~400ns

  // Wait for drive to become ready
  while (inb(0x1F7) & 0x80)
    ; // Wait for BSY to clear

  // Select master drive
  outb(0x1F6, 0xA0);
  for (int i = 0; i < 4; i++)
    inb(0x1F7); // Wait ~400ns

  // Wait for drive ready
  int timeout = 100000;
  while (!(inb(0x1F7) & 0x40) && timeout > 0) {
    timeout--;
  }
}

uint32_t *ATA_identify() {

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

  while (inb(0x1F7) & 0x80)
    ;
  while (!(inb(0x1F7) & 0x08))
    ;

  uint16_t *identify_addr = (uint16_t *)kmalloc(512);
  if (!identify_addr)
    return NULL;

  for (int i = 0; i < 256; i++) {
    identify_addr[i] = inw(0x1F0);
  }

  return (uint32_t *)identify_addr;
}

void mem_cpy(
    uint32_t *src, uint32_t *dest,
    uint32_t size) { // dunno if this is the correct file to have mem_cpy in
  for (uint32_t i = 0; i < size; i++) {
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

uint16_t *disk_read(uint32_t LBA, uint32_t sector_count) {
  outb(0x1F6, (0xE0 | ((LBA >> 24) & 0x0F)));

  for (int i = 0; i < 4; i++)
    inb(0x1F7); // provide a 400ns delay so that the DRQ can set

  outb(0x1F2, (unsigned char)(sector_count));
  outb(0x1F3, (unsigned char)(LBA));
  outb(0x1F4, (unsigned char)(LBA >> 8));
  outb(0x1F5, (unsigned char)(LBA >> 16));
  outb(0x1F7, 0x20);

  uint16_t *buffer = (uint16_t *)kmalloc(sector_count * 512);

  for (uint32_t i = 0; i < sector_count; i++) {
    while (inb(0x1F7) & 0x80)
      ;
    while (!(inb(0x1F7) & 0x08))
      ;

    for (int j = 0; j < 256; j++) {
      buffer[i * 256 + j] = inw(0x1F0);
    }
  }

  return buffer;
}

void disk_write(uint32_t LBA, uint32_t sector_count, uint16_t *buffer) {
  outb(0x1F6, (0xE0 | ((LBA >> 24) & 0x0F)));

  for (int i = 0; i < 4; i++)
    inb(0x1F7); // provide a 400ns delay so that the DRQ can set

  outb(0x1F2, (unsigned char)(sector_count));
  outb(0x1F3, (unsigned char)(LBA));
  outb(0x1F4, (unsigned char)(LBA >> 8));
  outb(0x1F5, (unsigned char)(LBA >> 16));
  outb(0x1F7, 0x30);

  for (uint32_t i = 0; i < sector_count; i++) {
    while (inb(0x1F7) & 0x80)
      ;
    while (!(inb(0x1F7) & 0x08))
      ;

    for (int j = 0; j < 256; j++) {
      outw(0x1F0, buffer[i * 256 + j]);
    }
  }

  outb(0x1F7, 0xE7);
  while (inb(0x1F7) & 0x80)
    ;
}

void fs_init() {
  ATA_init();

  sb = (struct superblock *)disk_read(1, 1);

  // check the magic number to see if the disk has previously been formatted
  // magic number is 0x19671972

  if (sb->magic_number == 0x19671972) {
    terminal_writestring("Filesystem detected.\n");
    disk_bitmap = (uint32_t *)disk_read(2, 1);

    sb->disk_bitmap_LBA = 2;

    directories = (struct directory *)disk_read(3, 1);
    sb->directories_LBA = 3;

    /* General idea so far:
    On startup we read the superblock from the disk
    if it is formatted we load the disk bitmap and directory array into memory
    in particular, the first entry in the directory array is the root directory
    in the block array in the inode struct we have 12 blocks that can either be
    assigned to a file or directory
    */
  } else {
    terminal_writestring("No filesystem detected. Formatting disk...\n");
    format_disk();
  }
}

void format_disk() {
  uint32_t *identify_addr = ATA_identify();

  if (identify_addr == 0) {
    terminal_writestring("Error: Disk not found or error.\n");
    return;
  }

  uint16_t *buffer = (uint16_t *)identify_addr;
  uint32_t sector_count = (uint32_t)buffer[60] | ((uint32_t)buffer[61] << 16);
  kfree(identify_addr);

  // format disk bitmap
  uint32_t bitmap_size_bytes = (sector_count + 7) / 8;
  if (bitmap_size_bytes % 4 != 0)
    bitmap_size_bytes += (4 - (bitmap_size_bytes % 4));
  disk_bitmap = (uint32_t *)kmalloc(bitmap_size_bytes);
  // set the first 3 sectors as used
  for (uint32_t i = 0; i < (sector_count + 31) / 32; i++)
    disk_bitmap[i] = 0x0;
  disk_bitmap[0] = 0x0F;

  sb->disk_bitmap_LBA = 2;
  sb->total_disk_size = sector_count;

  sb->magic_number = 0x19671972;

  // format the directories
  directories =
      (struct directory *)kmalloc(512); // allocate 1 sector for directory array
  strcpy(directories[0].name, "root");
  directories[0].inode.permission = 1;
  directories[0].inode.type = 0; // directory
  directories[0].inode.size = 0;
  sb->directories_LBA = 3;

  sb->num_inodes = 1;

  // now write everything to the disk
  // write superblock
  disk_write(1, 1, (uint16_t *)sb);

  // write disk bitmap
  uint32_t bitmap_sectors = (bitmap_size_bytes + 511) / 512;
  disk_write(sb->disk_bitmap_LBA, bitmap_sectors, (uint16_t *)disk_bitmap);

  // write directory array
  disk_write(sb->directories_LBA, 1, (uint16_t *)directories);
}
