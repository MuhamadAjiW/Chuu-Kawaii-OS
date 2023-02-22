#include "lib-header/portio.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/gdt.h"
#include "lib-header/framebuffer.h"
#include "lib-header/kernel_loader.h"
#include "cpu/idt.h"
#include "cpu/isr.h"

void kernel_setup(void) {
    enter_protected_mode(&_gdt_gdtr);
    init_idt();

    framebuffer_clear();
    /* Test the interrupts */
    __asm__ volatile ("int $2");
    
    /*
    framebuffer_write(3, 8,  'H', 0, 0xF);
    framebuffer_write(3, 9,  'a', 0, 0xF);
    framebuffer_write(3, 10, 'i', 0, 0xF);
    framebuffer_write(3, 11, '!', 0, 0xF);
    */
    //framebuffer_set_cursor(3, 10);
    //framebuffer_print("print test\n yes", 0, 0xf);
    
    while (TRUE);
}