
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
#define SYSCALL_CLEAR_SCREEN 6
#define SYSCALL_MOVE_CURSOR 7
#define SYSCALL_BACKSPACE 8
#define SYSCALL_READ_FILE 9

void syscall(registers r);
void activate_system_call();

#endif
