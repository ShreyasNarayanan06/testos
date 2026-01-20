#include <stddef.h>

extern char text_buffer[128];
extern size_t text_buffer_size;
extern char filename_buffer[128];
extern size_t filename_buffer_size;

void textedit();
void return_to_terminal();
void close_file();