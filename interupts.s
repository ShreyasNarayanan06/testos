.global IRQ1_handler
.global load_idt

IRQ1_handler:
    pusha /* pushes all registers to the stack */
    call keyboard_handler /* calls the keyboard_handler function in kernel.c */
    popa /* pops all registers from the stack */
    iret /* iret returns from an interrupt */

load_idt:
    mov 4(%esp), %eax   /* Get the pointer to the idt_ptr struct from the C argument. The eax register is defined as the first argument */
    lidt (%eax)         /* Load the IDT into the CPU register */
    ret