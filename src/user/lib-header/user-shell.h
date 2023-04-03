

#ifndef _USER_SHELL
#define _USER_SHELL

#define INPUT_BUFFER_SIZE 1024

#define NULL_CHAR 0
#define TAB_CHAR 1
#define LARROW_CHAR 2
#define RARROW_CHAR 3
#define BACKSPACE_CHAR 4

struct shell_reader
{
    char* keyboard_buffer;
    uint32_t buffersize;
    uint32_t maxIdx;
    uint32_t currentIdx;
};
typedef struct shell_reader shell_reader;

void initialize_shell();
void close_shell();
void clear_shell();
void evaluate_shell();
void append_shell(char in);
void move_shell(int direction);
void backspace_shell();
char* get_keyboard_buffer();
void newline_shell();


#endif