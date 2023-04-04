
#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "stdtype.h"

// Macros for system call codes, should always be synchronized with the kernel side
#define SYSCALL_IDLE 0
#define SYSCALL_MALLOC 1
#define SYSCALL_REALLOC 2
#define SYSCALL_FREE 3
#define SYSCALL_GET_KEYBOARD_LAST_KEY 4
#define SYSCALL_PRINT_STR 5
#define SYSCALL_PRINT_STR_C 6
#define SYSCALL_PRINT_CHAR 7
#define SYSCALL_PRINT_CHAR_C 8
#define SYSCALL_CLEAR_SCREEN 9
#define SYSCALL_MOVE_CURSOR 10
#define SYSCALL_LIMIT_CURSOR 11
#define SYSCALL_BACKSPACE 12
#define SYSCALL_READ_FILE 13
#define SYSCALL_READ_DIR 14
#define SYSCALL_SELF_DIR_INFO 15
#define SYSCALL_WRITE_FILE 16
#define SYSCALL_DELETE_FILE 17
#define SYSCALL_CLOSE_FILE 18
#define SYSCALL_CLOSE_DIR 19
#define SYSCALL_GET_CMOS_DATA 20

/**
 * Sends syscall to the kernel
 * @attention   make sure to add a response in the kernel side if not already
 *              eax should be filled with system call codes
 *              the rest may be filled with function arguments
*/
void syscall(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx);

#endif