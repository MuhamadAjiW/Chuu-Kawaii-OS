
#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "isr.h"

#define SYSCALL_COUNT 128

// Macros for system call codes, should always be synchronized with the user side
#define SYSCALL_IDLE 0
#define SYSCALL_ANIMATION 1
#define SYSCALL_MALLOC 2
#define SYSCALL_REALLOC 3
#define SYSCALL_FREE 4
#define SYSCALL_GET_KEYBOARD_LAST_KEY 5
#define SYSCALL_PRINT_STR 6
#define SYSCALL_PRINT_STR_C 7
#define SYSCALL_PRINT_CHAR 8
#define SYSCALL_PRINT_CHAR_C 9
#define SYSCALL_CLEAR_SCREEN 10
#define SYSCALL_MOVE_CURSOR 11
#define SYSCALL_LIMIT_CURSOR 12
#define SYSCALL_BACKSPACE 13
#define SYSCALL_READ_FILE 14
#define SYSCALL_READ_DIR 15
#define SYSCALL_SELF_DIR_INFO 16
#define SYSCALL_WRITE_FILE 17
#define SYSCALL_DELETE_FILE 18
#define SYSCALL_CLOSE_FILE 19
#define SYSCALL_CLOSE_DIR 20
#define SYSCALL_GET_CMOS_DATA 21
#define SYSCALL_GETTICK 22

/**
 * Assigns system response functions to system call number
 * 
*/
void enable_system_calls();

/**
 * Call assigned response
 * 
 * @param r Sent register values
 * 
*/
void syscall_response(registers r);


/**
 * 
 * Activate syscall interrupt and
 * Assign response functions into syscall codes 
 * 
*/
void activate_system_call();

#endif
