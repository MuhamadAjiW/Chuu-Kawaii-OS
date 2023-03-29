#include "../lib-header/syscall.h"
#include "../lib-header/isr.h"

#include "../lib-header/graphics.h"


static void* syscalls[] = {
    (void*) graphics_scroll
};

void syscall(registers regs){
    uint32_t retval;
    __asm__ volatile ("push %0" : : "r"(regs.edi));
    __asm__ volatile ("push %0" : : "r"(regs.esi));
    __asm__ volatile ("push %0" : : "r"(regs.edx));
    __asm__ volatile ("push %0" : : "r"(regs.ecx));
    __asm__ volatile ("push %0" : : "r"(regs.ebx));
    __asm__ volatile ("call %0" : : "r"(syscalls[regs.eax]));
    __asm__ volatile ("pop %ebx");
    __asm__ volatile ("pop %ebx");
    __asm__ volatile ("pop %ebx");
    __asm__ volatile ("pop %ebx");
    __asm__ volatile ("pop %%ebx" : "=a"(retval));
}

void activate_system_call(){
    register_interrupt_handler(0x30, syscall);
}
