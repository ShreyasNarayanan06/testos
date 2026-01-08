/*
General idea:
every time a character is pressed its added to a buffer in the case statement
then when enter is pressed the buffer is checked for a command
*/

#include "../../headers/terminal.h"
#include <stddef.h>

char command_buffer[128];
size_t curr_size = 0;

char command[128];
char arg[128];

int str_cmp(char *str1, char *str2) {
    size_t len2 = 0;
    while (str2[len2]) {
        len2++;
    }

    size_t len1 = 0;
    while (str1[len1]) {
        len1++;
    }

    if(len1 != len2) return 0;

    for(size_t i = 0; i < len1; i++) {
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

void command_buffer_removechar() {
    command_buffer[curr_size] = '\0';
    curr_size--;
}

void parse_input() {
    int argflag = 0;
    int cmd_idx = 0;
    int arg_idx = 0;

    for(size_t i = 0; i < curr_size; i++) {
        if(command_buffer[i] == ' ' && !argflag) {
            argflag = 1;
            continue;
        }

        if(!argflag) {
            command[cmd_idx++] = command_buffer[i];
        } else {
            arg[arg_idx++] = command_buffer[i];
        }
    }

    command[cmd_idx] = '\0';
    arg[arg_idx] = '\0';
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
    terminal_writestring("echo: repeat the argument");
    terminal_enter_no_prompt();
    terminal_enter();
}

void command_echo() {
    terminal_enter_no_prompt();
    terminal_writestring(arg);
    terminal_enter();
}

void command_info() {
    terminal_enter_no_prompt();
    terminal_writestring("testos | v0.1 | naray | 1/8/2026");
    terminal_enter();
}

void command_usr() {
    terminal_enter_no_prompt();
    terminal_writestring("User: naray");
    terminal_enter();
}

// int checkspace() {
//     for(size_t i = 0; i < curr_size; i++) {
//         if(command[i] == ' ') {
//             return 1;
//         }
//     }
//     return 0;
// }

void check_command() {
    parse_input();
    
  if(str_cmp(command_buffer, "clear")) {
    terminal_clear();
  } else if (str_cmp(command_buffer, "help")) {
    command_help();
  } else if(str_cmp(command, "echo")) {
    command_echo();
  } else if(str_cmp(command, "info")) {
    command_info();
  } else if(str_cmp(command, "usr")) {
    command_usr();
  } else { //needs work
    // terminal_writenumber(checkspace());
    terminal_enter_no_prompt();
    terminal_writestring("Command Not Found: ");
    terminal_writestring(command_buffer);
    terminal_enter();
    }
}


