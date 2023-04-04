#ifndef _ISR_H
#define _ISR_H

#include "stdtype.h"

/* further reading:
      https://wiki.osdev.org/Interrupt_Descriptor_Table
      https://wiki.osdev.org/Interrupts_Tutorial
*/

/**
 *  Struct to pass register values
 *  Should be self explanatory 
 * 
 */
struct registers{
   uint32_t ds;
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
   uint32_t int_no, err_code;
   uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed));
typedef struct registers registers;


/**
 *  Function struct with register as its parameter
 * 
 */
typedef void (*InterruptService)(registers);

/**
 *  Assign default interrupt response to interrupt number
 *  @warning            only viable for interrupt number larger than 31 as 0-31 is reserved for exception and is handled manually
 * 
 *  @param int_no       interrupt number
 *  @param function     function to be assigned
 */
void register_interrupt_handler(uint8_t int_no, InterruptService function);

#endif