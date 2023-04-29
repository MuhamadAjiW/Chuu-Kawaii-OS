

#ifndef _USER_SHELL
#define _USER_SHELL

#define INPUT_BUFFER_SIZE 1024

#define NULL_CHAR 0
#define TAB_CHAR 1
#define LARROW_CHAR 2
#define RARROW_CHAR 3
#define UARROW_CHAR 4
#define DARROW_CHAR 5
#define ESC_CHAR 6
#define BACKSPACE_CHAR 7

/**
 *  Singleton struct to store reader information
 *  
 *  @param keyboard_buffer      dynamic buffer for stored text
 *  @param buffersize           current keyboard_buffer size
 *  @param maxIdx               total length of written characters
 *  @param currentIdx           location of cursor relative to the reader
 */
struct shell_reader
{
    char* keyboard_buffer;
    uint32_t buffersize;
    uint32_t maxIdx;
    uint32_t currentIdx;
};
typedef struct shell_reader shell_reader;

/**
 * Struct containing the value of bytes in a cluster
 * 
 * @param directory_path           Directory Path
 * @param cluster_number           Cluster Number 
 */
struct directory_info{
    char* directory_path;
    uint32_t cluster_number;
}__attribute__((packed));
typedef struct directory_info directory_info;

/**
 *  Allocates keyboard_buffer with the default size
 */
void initialize_shell();

/**
 *  Unallocates keyboard_buffer
 */
void close_shell();

/**
 *  Clears keyboard_buffer and reset it to a new address
 *  Set maxIdx and currentIdx to 0
 */
void clear_shell();

/**
 *  Parses keyboard_buffer and executes appropriate commands
 */
void evaluate_shell();

/**
 *  Adds a char to the keyboard_buffer
 *  Evaluates shell if inserted char is '\n'
 * 
 *  @param in               inserted char
 */
void append_shell(char in);

/**
 *  Moves cursor relative to the reader
 * 
 *  @param direction        movement direction. 1 for right, -1 for left
 */
void move_shell(int direction);

/**
 * Deletes 1 character behind the cursor
 * 
 * @attention this function only handles the reader, the graphics is handled by backspace() in commands
 */
void backspace_shell();

/**
 *  getter for keyboard_buffer, might be useful for parsing
 * 
 *  @return                 keyboard_buffer
 */
char* get_keyboard_buffer();

/**
 *  Prints a newline for the shell
 */
void newline_shell();

#endif