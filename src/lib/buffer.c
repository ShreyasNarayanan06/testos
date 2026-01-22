#include <stddef.h>

void fill_buffer(char c, size_t* curr_size, char* buffer) {
  buffer[*curr_size] = c;
  (*curr_size)++;
  // terminal_enter();
  // terminal_writestring(command_buffer);
}

void clear_buffer(size_t* curr_size, char* buffer) {
  for(size_t i = 0; i < *curr_size; i++) {
    buffer[i] = '\0';
  }
  *curr_size = 0;
}

void command_buffer_removechar(size_t* curr_size, char* buffer) {
    if (*curr_size > 0) {
        (*curr_size)--;          
        buffer[*curr_size] = '\0';
    }
}