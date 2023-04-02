#ifndef _ISR_H
#define _ISR_H

#include "stdtype.h"

struct registers{
   uint32_t ds;
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
   uint32_t int_no, err_code;
   uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed));
typedef struct registers registers;
typedef void (*InterruptService)(registers);

void isr_handler(registers);
void irq_handler(registers);
void register_interrupt_handler(uint8_t, InterruptService input);

void syscall_response(registers r);

#endif