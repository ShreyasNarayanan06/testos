#include <stdint.h>
#include <stddef.h>

extern size_t t_row;
extern size_t t_col;
extern uint8_t t_color;
extern uint16_t* t_buffer;

void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_removechar();
void terminal_enter();
void terminal_arrow_right();
void terminal_arrow_left();
// void terminal_arrow_up();
// void terminal_arrow_down();
void terminal_writenumber(uint32_t n);
void terminal_clear();


