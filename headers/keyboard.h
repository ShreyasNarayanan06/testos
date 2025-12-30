#include <stdint.h>
#include "terminal.h"

extern int shift_pressed;
extern int is_extended;

void process_keyboard_input(uint8_t scancode);
void keyboard_handler(void);
void reboot();
