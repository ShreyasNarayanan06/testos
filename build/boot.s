/* Declare constants for the multiboot header. */
.set ALIGN,    1<<0 /* Sets the align to 0 */            
.set MEMINFO,  1<<1 /* Sets the memory info to 1 */            
.set FLAGS,    ALIGN | MEMINFO /* Sets the memory flags to 0011*/ 
.set MAGIC,    0x1BADB002 /* Sets the magic number to 0x1BADB002 */       
.set CHECKSUM, -(MAGIC + FLAGS) /* Sets the checksum to the negative of the magic number and flags, MAGIC + FLAGS + CHECKSUM = 0,0x1BADB002 + 0x00000003 + CHECKSUM = 0 */ 

/* Declare the multiboot header. uses the global variables that we set before to create the header,
 the align makes sure that the header is aligned to 4 bytes */
.section .multiboot, "a"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Define the stack information so we have something for the sp to point to */
.section .bss /* .bss defines any section of uninitialized data, in this case it is the stack but it can be used generally for any uninitialized data */
.align 16 /* Aligns the stack to 16 bytes */
stack_bottom:
.skip 16384 /* Skips 16384 bytes */
stack_top: /* this defines a grow down stack because we allocate memory from the bottom up */


/* Define a section for the start of the kernel code, this actually handles moving the code into memory */
.section .text /* .text defines any section of initialized data, in this case it is the kernel code but it can be used generally for any initialized data */
.global _start /* global makes the symbol visible to the linker */
_start: 
    
    /*
	The bootloader has loaded us into 32-bit protected mode on a x86
	machine. Interrupts are disabled. Paging is disabled. The processor
	state is as defined in the multiboot standard. The kernel has full
	control of the CPU. The kernel can only make use of hardware features
	and any code it provides as part of itself. There's no printf
	function, unless the kernel provides its own <stdio.h> header and a
	printf implementation. There are no security restrictions, no
	safeguards, no debugging mechanisms, only what the kernel provides
	itself. It has absolute and complete power over the
	machine.
	*/

    /* moves the stack top to the stack pointer, esp is defined in the x86-32 ABI as the stack pointer */
    mov $stack_top, %esp 

    call kernel_main /* calls the kernel_main function, call is from the x86-32 ABI, kernel_main is a symbol */

    cli /* cli disables interrupts */
1:  hlt /* hlt halts the processor */
    jmp 1b /* jmp jumps to the label 1, this creates an infinite loop */
 
.size _start, . - _start /* sets the size of the _start section to the size of the _start section */