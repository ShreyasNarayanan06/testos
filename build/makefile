# Compiler & Linker
CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc

# Flags
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I../headers
LDFLAGS = -ffreestanding -O2 -nostdlib -T linker.ld -lgcc

# Object Files
OBJ = boot.o \
      ../src/kernel.o \
      ../src/cpu/idt.o \
      ../src/cpu/pic.o \
      ../src/memory/manual_management/PMM.o \
      ../src/memory/manual_management/VMM.o \
      ../src/memory/user_interaction/fs.o \
      ../src/driver/keyboard.o \
      ../src/driver/terminal.o \
	  ../src/driver/commands.o \
      ../assembly/interupts.o \
      ../assembly/VMM.o

# Final Binary
os.bin: $(OBJ)
	$(LD) -o $@ $(OBJ) $(LDFLAGS)

# Compile C Files
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Compile Boot Assembly
boot.o: boot.s
	$(AS) $< -o $@

# Compile Assembly Wrappers
../assembly/%.o: ../assembly/%.s
	$(AS) $< -o $@

# Run OS in QEMU
run: os.bin
	qemu-system-i386 -kernel os.bin

# Clean
clean:
	rm -f os.bin
	rm -f *.o
	rm -f ../src/*.o
	rm -f ../src/cpu/*.o
	rm -f ../src/memory/*.o
	rm -f ../src/memory/manual_management/*.o
	rm -f ../src/memory/user_interaction/*.o
	rm -f ../src/driver/*.o
	rm -f ../assembly/*.o