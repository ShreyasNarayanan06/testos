#include <stddef.h>

void check_command();
int str_cmp(char *str1, char *str2);
void command_help();
void command_info();
void command_usr();
void command_shutdown();
void command_textedit();

extern char command_buffer[128];
extern size_t curr_size;

