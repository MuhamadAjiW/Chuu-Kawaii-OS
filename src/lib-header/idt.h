#ifndef _IDT_H
#define _IDT_H

#include "stdtype.h"

#define IDT_MAX_COUNT 256
#define SELECTOR 0x08
#define KERNEL_FLAGS 0b10001110
#define USER_FLAGS 0b11101110

/* further reading:
      https://wiki.osdev.org/Interrupt_Descriptor_Table
      https://wiki.osdev.org/Interrupts_Tutorial
*/

/**
 *  Struct for IDT Entries
 *  @param base_low     callback offset bits, 0-15
 *  @param selector     code segment selector in GDT
 *  @param reserved     free for programmers to use
 *  @param flags        bits for flags
 *                       bit 0-3 determines gate type
 *                       bit 4 should always be set to 0
 *                       bit 5-6 determines CPU privilege levels
 *                       bit 7 marks if it is present
 *  @param base_high    callback offset bits, 16-31
 */
struct InterruptDescriptor{
   uint16_t base_low;             
   uint16_t selector;
   uint8_t  reserved;
   uint8_t  flags;
   uint16_t base_high;
} __attribute__((packed));
typedef struct InterruptDescriptor InterruptDescriptor;

/**
 *  Struct containing IDT Entries
 *  Should be self explanatory
 */
struct InterruptDescriptorTable{
    InterruptDescriptor table[IDT_MAX_COUNT];
} __attribute__((packed));
typedef struct InterruptDescriptorTable InterruptDescriptorTable;

/**
 *  Singleton struct containing IDTR to be loaded to the CPU
 *  @param size size of InterruptDescriptorTable - 1
 *  @param address address of loaded Interrupt Descriptor Table
 */
struct IDTR{
   uint16_t size;
   InterruptDescriptorTable* address;
} __attribute__((packed));
typedef struct IDTR IDTR;


/**
 *  Reference to assembly function loading the IDTR
 * 
 *  @param address address of IDTR
 */
extern void load_idt(IDTR* address);


/**
 *  Sets gates for system IDT entries then loads IDT
 * 
 */
void initialize_idt();

/**
 *  Sets gates for every an IDT entry
 * 
 *  @param n         interrupt number
 *  @param input     address for interrupt callback, callback is generalized to stubs to simplify operations
 *  @param flags     flags for the IDT entry
 */
void set_idt_gate(uint8_t n, void* input, uint8_t flags);

/**
 *  Remaps IRQ number to 32-47
 * 
 */
void pic_remap();

#endif