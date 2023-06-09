
#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "stdtype.h"

// Macros for system call codes, should always be synchronized with the kernel side
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
#define SYSCALL_AS_DIRECTORY 23
#define SYSCALL_IS_DIRECTORY 24
#define SYSCALL_NAME_EXISTS 25






#define SYSCALL_DISPLAY_TEXT 64
#define SYSCALL_GET_CURSOR_Y 65
#define SYSCALL_GET_CURSOR_X 66
#define SYSCALL_GET_TEXT_EDGE 67


/**
 * Sends syscall to the kernel
 * @attention   make sure to add a response in the kernel side if not already
 *              eax should be filled with system call codes
 *              the rest may be filled with function arguments
*/
void syscall(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx);

#endif