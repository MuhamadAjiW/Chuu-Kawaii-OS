#ifndef _IDT_H
#define _IDT_H

#include "stdtype.h"

#define IDT_MAX_COUNT 256
#define SELECTOR 0x08
#define KERNEL_FLAGS 0b10001110
#define USER_FLAGS 0b11101110

struct InterruptDescriptor{
   uint16_t base_low;             
   uint16_t selector;
   uint8_t  reserved;
   uint8_t  flags;
   uint16_t base_high;
} __attribute__((packed));
typedef struct InterruptDescriptor InterruptDescriptor;

struct InterruptDescriptorTable{
    InterruptDescriptor table[IDT_MAX_COUNT];
} __attribute__((packed));
typedef struct InterruptDescriptorTable InterruptDescriptorTable;

struct IDTR{
   uint16_t size;
   InterruptDescriptorTable* address;
} __attribute__((packed));
typedef struct IDTR IDTR;

extern void load_idt(IDTR*);
void initialize_idt();
void set_idt_gate(uint8_t n, void* input, uint8_t flags);
void pic_remap();

void enable_system_calls();

#endif