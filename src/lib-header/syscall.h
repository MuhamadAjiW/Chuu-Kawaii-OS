
#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "isr.h"

#define SYSCALL_COUNT 128

#define SYSCALL_IDLE 0
#define SYSCALL_MALLOC 1
#define SYSCALL_REALLOC 2
#define SYSCALL_FREE 3
#define SYSCALL_GET_KEYBOARD_LAST_KEY 4
#define SYSCALL_PRINT_STR 5
#define SYSCALL_PRINT_CHAR 6
#define SYSCALL_CLEAR_SCREEN 7
#define SYSCALL_MOVE_CURSOR 8
#define SYSCALL_LIMIT_CURSOR 9
#define SYSCALL_BACKSPACE 10
#define SYSCALL_READ_FILE 11
#define SYSCALL_READ_DIR 12
#define SYSCALL_WRITE_FILE 13
#define SYSCALL_DELETE_FILE 14

void syscall_response(registers r);
void activate_system_call();

#endif
