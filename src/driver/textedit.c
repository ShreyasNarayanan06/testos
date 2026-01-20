/*General Ideas
- I need to clear the screen
- I need to store everything the user is typing in a buffer
- when the user hits a specific combination of keys, it returns to the terminal and stores the buffer in a file
- before I return to the terminal, I need to ask the user for a filename
- To return to the terminal, I need to store its state. This means I need to save everythign in the terminal in a return buffer
*/

/* Process:
- Textedit is called
- Clear Screen
- Create new buffer to store all information
- wait for termination command
- ask for filename
- kmalloc the buffer
- make an inode in root
- store the buffer in the inode
- store to disk
- return to the terminal
*/

#include "../../headers/terminal.h"
#include "../../headers/VMM.h"
#include "../../headers/keyboard.h"

char text_buffer[128];
size_t text_buffer_size = 0;

char filename_buffer[128];
size_t filename_buffer_size = 0;

void textedit() {
    curr_buffer = TEXT_BUFFER;
    terminal_clear_TE();  
//  terminal_writestring("Enter your text. Press Ctrl+E to save and exit.");
}

void return_to_terminal() {
	curr_buffer = COMMAND_BUFFER;
	terminal_initialize();
    // terminal_enter();
    // terminal_writestring("here is the buffer:");
    // terminal_enter_no_prompt();
    // terminal_writestring(text_buffer);
}

void close_file() {
    terminal_set_cursor(0, VGA_HEIGHT-2);
    terminal_writestring("Enter filename: ");
    curr_buffer = FILENAME_BUFFER;
}



