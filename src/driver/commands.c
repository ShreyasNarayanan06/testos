/*
General idea:
every time a character is pressed its added to a buffer in the case statement
then when enter is pressed the buffer is checked for a command
*/

#include "../../headers/terminal.h"
#include <stddef.h>

char *command_buffer = "";
size_t curr_size = 0;

int str_cmp(char *str1, char *str2) {
  for(size_t i = 0; i < curr_size; i++) {
    if(str1[i] != str2[i]) return 0;
  }
  return 1;
}

void fill_buffer(char c) {
  command_buffer[curr_size] = c;
  curr_size++;
  // terminal_enter();
  // terminal_writestring(command_buffer);
}

void clear_buffer() {
  command_buffer = "";
  curr_size = 0;
}

void check_command() {
  if(str_cmp(command_buffer, "clear")) {
    terminal_clear();
  }
}

