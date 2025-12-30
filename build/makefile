# Compiler & Linker
CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc

# Flags
# -I../headers tells the compiler to look in the headers folder for .h files
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I../headers
LDFLAGS = -ffreestanding -O2 -nostdlib -T linker.ld -lgcc

# Object Files
OBJ = boot.o \
      ../src/kernel.o \
      ../src/cpu/idt.o \
      ../src/cpu/pic.o \
      ../src/driver/keyboard.o \
      ../src/driver/terminal.o \
      ../assembly/interupts.o

# Final Binary
os.bin: $(OBJ)
	$(LD) -o $@ $(OBJ) $(LDFLAGS)

# Compile C Files
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Compile Boot Assembly (in same folder)
boot.o: boot.s
	$(AS) $< -o $@

# Compile Assembly Wrappers (in assembly folder)
../assembly/interupts.o: ../assembly/interupts.s
	$(AS) $< -o $@

# Run OS in QEMU
run: os.bin
	qemu-system-i386 -kernel os.bin

# Remove all binaries and object files
clean:
	rm -f os.bin
	rm -f *.o
	rm -f ../src/*.o
	rm -f ../src/cpu/*.o
	rm -f ../src/driver/*.o
	rm -f ../assembly/*.o