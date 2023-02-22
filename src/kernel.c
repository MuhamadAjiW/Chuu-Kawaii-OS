#include "lib-header/portio.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/gdt.h"
#include "lib-header/framebuffer.h"
#include "lib-header/kernel_loader.h"
#include "cpu/idt.h"
#include "cpu/isr.h"
#include "cpu/timer.h"
#include "drivers/keyboard.h"

#include "shell.h"

void kernel_setup(void) {
    enter_protected_mode(&_gdt_gdtr);
    init_idt();

    framebuffer_clear();
    __asm__ volatile ("sti");
    
    //init_timer(20); // Harus > 18, kalo gak rada unpredictable
    init_shell();
    
    /*
    char buffer[3];
    buffer[1] = '\n';
    buffer[2] = 0;
    for(int i = 0; i < 128; i++){
        buffer[0] = i;
        framebuffer_printDef(buffer);
    }
    */

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