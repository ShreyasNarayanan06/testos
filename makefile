# 1. Variables
CC = i686-elf-gcc
AS = i686-elf-as
LNK = i686-elf-gcc

# 2. Flags
# -ffreestanding: No standard library
# -g: Include debug symbols for QEMU/GDB
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -g
LDFLAGS = -ffreestanding -O2 -nostdlib -T linker.ld -lgcc

# 3. File Discovery
C_SOURCES = $(wildcard *.c)
S_SOURCES = $(wildcard *.s)
OBJ = $(C_SOURCES:.c=.o) $(S_SOURCES:.s=.o)

# 4. Main Targets
all: os.bin

os.bin: $(OBJ)
	$(LNK) -o $@ $^ $(LDFLAGS)

# Rule for C files
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Rule for Assembly files (boot.s, interrupts.s, etc.)
%.o: %.s
	$(AS) $< -o $@

# 5. Utility Targets
run: os.bin
	qemu-system-i386 -kernel os.bin

clean:
	rm -f *.o os.bin