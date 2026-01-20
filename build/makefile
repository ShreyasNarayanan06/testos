# Compiler & Linker
CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc

# Flags
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I../headers
LDFLAGS = -ffreestanding -O2 -nostdlib -T linker.ld -lgcc

# --- AUTO-DETECTION START ---
# 1. Find all .c files in ../src (recursive)
C_SOURCES = $(shell find ../src -name "*.c")

# 2. Find all .s files in ../assembly (recursive)
ASM_SOURCES = $(shell find ../assembly -name "*.s")

# 3. Convert source lists to object lists (.c -> .o, .s -> .o)
C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)

# 4. Combine them (boot.o MUST often come first depending on linker script)
OBJ = boot.o $(C_OBJECTS) $(ASM_OBJECTS)
# --- AUTO-DETECTION END ---

# Final Binary
os.bin: $(OBJ)
	$(LD) -o $@ $(OBJ) $(LDFLAGS)

# Generic Rule for C files (Works for any path depth)
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Compile Boot Assembly (Specific rule since it's in build dir)
boot.o: boot.s
	$(AS) $< -o $@

# Generic Rule for Assembly files in ../assembly/
# Note: This pattern match ensures we only pick up the external assembly files
../assembly/%.o: ../assembly/%.s
	$(AS) $< -o $@

# Run OS in QEMU
disk.img:
	dd if=/dev/zero of=disk.img bs=1M count=10

run: os.bin disk.img
	qemu-system-i386 -machine pc -kernel os.bin -drive file=disk.img,format=raw,if=ide,index=0,media=disk

# Clean
clean:
	rm -f os.bin
	rm -f *.o
	rm -f $(C_OBJECTS) $(ASM_OBJECTS)