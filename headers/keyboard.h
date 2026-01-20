#include <stdint.h>
#include "terminal.h"

extern int shift_pressed;
extern int is_extended;
extern char* buffer;
extern size_t* buffer_size;

void process_keyboard_input(uint8_t scancode);
void keyboard_handler(void);
void reboot();
void set_buffer(); 

enum buffer_type {
    COMMAND_BUFFER,
    TEXT_BUFFER,
    FILENAME_BUFFER
};

extern enum buffer_type curr_buffer;
