#include "lib-header/portio.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/gdt.h"
#include "lib-header/framebuffer.h"
#include "lib-header/kernel_loader.h"
#include "lib-header/idt.h"
#include "lib-header/isr.h"
#include "lib-header/timer.h"
#include "lib-header/keyboard.h"
#include "lib-header/string.h"
#include "lib-header/fat32.h"
#include "lib-header/disk.h"

#include "lib-header/shell.h"


uint32_t* target;
uint32_t entry[512];
char buffer[128]; 

void kernel_setup(void) {
    
    enter_protected_mode(&_gdt_gdtr);
    pic_remap();
    initialize_idt();

    activate_keyboard_interrupt();
    activate_timer_interrupt(20); // Harus > 18, kalo gak rada unpredictable
    
    framebuffer_clear();
    
    read_blocks(target, 0, 1);
    for(int i = 0; i < 128; i++){
        int_toString((target[i]) >> 24, buffer);
        framebuffer_printDef(buffer);
        framebuffer_printDef(" ");
        int_toString(((target[i]) >> 16) & 0xff, buffer);
        framebuffer_printDef(buffer);
        framebuffer_printDef(" ");
        int_toString(((target[i]) >> 8) & 0xff, buffer);
        framebuffer_printDef(buffer);
        framebuffer_printDef(" ");
        int_toString((target[i]) & 0xff, buffer);
        framebuffer_printDef(buffer);
        framebuffer_printDef(" ");
    }

    initialize_filesystem_fat32();
    
    framebuffer_clear();
    read_blocks(target, cluster_to_lba(2), 1);
    for(int i = 0; i < 128; i++){
        int_toString((target[i]) >> 24, buffer);
        framebuffer_printDef(buffer);
        framebuffer_printDef(" ");
        int_toString(((target[i]) >> 16) & 0xff, buffer);
        framebuffer_printDef(buffer);
        framebuffer_printDef(" ");
        int_toString(((target[i]) >> 8) & 0xff, buffer);
        framebuffer_printDef(buffer);
        framebuffer_printDef(" ");
        int_toString((target[i]) & 0xff, buffer);
        framebuffer_printDef(buffer);
        framebuffer_printDef(" ");
    }
    

    framebuffer_clear();
    keyboard_state_activate();
    
    init_shell();

    while (TRUE);
}