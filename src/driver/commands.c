/*
General idea:
every time a character is pressed its added to a buffer in the case statement
then when enter is pressed the buffer is checked for a command
*/

#include "../../headers/terminal.h"
#include "../../headers/string.h"
#include <stddef.h>
#include "../../headers/io.h"
#include "../../headers/disk.h"
#include "../../headers/textedit.h"

char command_buffer[128];
size_t curr_size = 0;

char command[128];
char arg[128];

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
    terminal_writestring("echo <text>: repeat the argument");
    terminal_enter_no_prompt();
    terminal_writestring("info: display OS version, release date, and author information");
    terminal_enter_no_prompt();
    terminal_writestring("usr: display current user");
    terminal_enter_no_prompt();
    terminal_writestring("shutdown: flush disk and exit QEMU");
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

void command_shutdown() {
    terminal_enter_no_prompt();
    terminal_writestring("Shutting down...");
    terminal_enter_no_prompt();
    
    // Flush filesystem writes first
    disk_write(1, 1, (uint16_t*)sb);
    
    // QEMU exit: writing to port 0x604 triggers ACPI shutdown
    outw(0x604, 0x2000);
}

void command_theme() {
  if(str_cmp(arg, "red")) {
    terminal_set_background(VGA_COLOR_RED);
  } else if(str_cmp(arg, "blue")) {
    terminal_set_background(VGA_COLOR_BLUE);
  } else if(str_cmp(arg, "green")) {
    terminal_set_background(VGA_COLOR_GREEN);
  } else if(str_cmp(arg, "magenta")) {
    terminal_set_background(VGA_COLOR_MAGENTA);
  } else if(str_cmp(arg, "cyan")) {
    terminal_set_background(VGA_COLOR_CYAN);
  } else if(str_cmp(arg, "white")) {
    terminal_set_background(VGA_COLOR_WHITE);
  }

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
  } else if(str_cmp(command, "shutdown")) {
    command_shutdown();
  } else if(str_cmp(command, "theme")) {
    command_theme();
  } else if(str_cmp(command, "te")) {
    textedit();
  } else { //needs work
    // terminal_writenumber(checkspace());
    terminal_enter_no_prompt();
    terminal_writestring("Command Not Found: ");
    terminal_writestring(command_buffer);
    terminal_enter();
    }
}


