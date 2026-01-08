/*
General idea:
every time a character is pressed its added to a buffer in the case statement
then when enter is pressed the buffer is checked for a command
*/

#include "../../headers/terminal.h"
#include <stddef.h>

char command_buffer[128];
size_t curr_size = 0;

int str_cmp(char *str1, char *str2) {
    size_t len2 = 0;
    while (str2[len2]) {
        len2++;
    }

    if(curr_size != len2) return 0;

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
  for(size_t i = 0; i < curr_size; i++) {
    command_buffer[i] = '\0';
  }
  curr_size = 0;
}

void command_help() {
    terminal_enter_no_prompt();
    terminal_writestring("Available Commands: ");
    terminal_enter_no_prompt();
    terminal_enter_no_prompt();
    terminal_writestring("help: display this message");
    terminal_enter_no_prompt();
    terminal_writestring("clear: clear the terminal");
    terminal_enter_no_prompt();
    terminal_enter();
}

void check_command() {
  if(str_cmp(command_buffer, "clear")) {
    terminal_clear();
  } else if (str_cmp(command_buffer, "help")) {
    command_help();
  } else {
    terminal_enter_no_prompt();
    terminal_writestring("Command Not Found: ");
    terminal_writestring(command_buffer);
    terminal_enter();
    }
}


