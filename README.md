# TestOS

A minimal x86 operating system kernel built from scratch in C and Assembly.

## Overview

TestOS is a hobbyist operating system targeting the x86 (i686) architecture. It boots via the Multiboot specification (compatible with GRUB/QEMU) and implements fundamental OS components including interrupt handling, memory management, and basic I/O.

## Features

- **Multiboot Compliant** – Boots with GRUB or directly via QEMU's `-kernel` flag
- **VGA Text Mode Terminal** – 80x25 character display with cursor support and multiple colors
- **Keyboard Driver** – Full PS/2 keyboard input with shift/caps lock support
- **Interrupt Handling** – IDT setup with PIC remapping for hardware interrupts
- **Physical Memory Manager (PMM)** – Bitmap-based page allocator using Multiboot memory maps
- **Virtual Memory Manager (VMM)** – Page directory/table setup with demand paging and page fault handling
- **Heap Allocator** – Simple `kmalloc`/`kfree` implementation for dynamic memory allocation

## Project Structure

```
testos/
├── build/                  # Build artifacts and configuration
│   ├── boot.s              # Multiboot header and entry point
│   ├── linker.ld           # Linker script (kernel loads at 1MB)
│   └── makefile            # Build system
├── src/
│   ├── kernel.c            # Kernel entry point (kernel_main)
│   ├── cpu/
│   │   ├── idt.c           # Interrupt Descriptor Table setup
│   │   └── pic.c           # Programmable Interrupt Controller initialization
│   ├── driver/
│   │   ├── terminal.c      # VGA text mode driver
│   │   └── keyboard.c      # PS/2 keyboard driver
│   └── memory/
│       ├── PMM.c           # Physical memory manager (bitmap allocator)
│       └── VMM.c           # Virtual memory manager (paging, heap)
├── headers/                # Header files
│   ├── idt.h, pic.h        # CPU headers
│   ├── terminal.h, keyboard.h, io.h  # Driver headers
│   └── PMM.h, VMM.h        # Memory management headers
└── assembly/               # Low-level assembly routines
    ├── interupts.s         # IRQ handler wrapper, IDT loading
    └── vmem.s              # Paging control (CR0/CR3, TLB flush)
```

## Prerequisites

You'll need an **i686-elf cross-compiler toolchain**:

- `i686-elf-gcc` – Cross-compiler for i686 target
- `i686-elf-as` – GNU Assembler for i686
- `qemu-system-i386` – Emulator for testing

## Building

```bash
cd build
make
```

This produces `os.bin`, a Multiboot-compliant kernel image.

## Running

```bash
cd build
make run
```

Or directly with QEMU:

```bash
qemu-system-i386 -kernel build/os.bin
```

## Boot Sequence

1. **Bootloader** – GRUB/QEMU loads the kernel at 1MB and passes control to `_start`
2. **Stack Setup** – 16KB stack initialized; Multiboot info pointer pushed to stack
3. **kernel_main()** – Main entry point:
   - Initialize VGA terminal
   - Set up IDT and remap PIC
   - Initialize physical memory manager (PMM) using Multiboot memory map
   - Initialize heap allocator
   - Enable paging (VMM)
   - Enable interrupts (`sti`) and enter idle loop

## Memory Layout

| Region | Address | Description |
|--------|---------|-------------|
| Kernel | 1MB+ | Code, data, BSS sections |
| Page Bitmap | 0x200000 | Physical page allocation bitmap |
| Heap | 0xD0000000 - 0xE0000000 | Kernel heap (demand-paged) |
| Recursive PD | 0xFFFFF000 | Self-mapped page directory |

## Keyboard Shortcuts

| Key | Action |
|-----|--------|
| Arrow keys | Move cursor |
| Backspace | Delete character |
| Enter | New line |
| Escape | Reboot system |

## Cleaning

```bash
cd build
make clean
```

## License

This project is for educational purposes.

---

*A learning project exploring low-level OS development on x86.*
