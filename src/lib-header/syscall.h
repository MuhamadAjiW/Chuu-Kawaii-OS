
#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "../lib-header/isr.h"

void syscall(registers a);
void activate_system_call();

#endif
