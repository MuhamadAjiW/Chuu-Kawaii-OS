#include "lib-header/portio.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/gdt.h"
#include "lib-header/kernel_loader.h"
#include "lib-header/idt.h"
#include "lib-header/isr.h"
#include "lib-header/timer.h"
#include "lib-header/keyboard.h"
#include "lib-header/string.h"
#include "lib-header/fat32.h"
#include "lib-header/disk.h"
#include "lib-header/memory_manager.h"
#include "lib-header/parser.h"
#include "lib-header/graphics.h"

#include "lib-header/shell.h"

/*
//debug purposes
uint32_t target[128] = {0};
uint32_t entry[512];
char buffer[128]; 

*/

void kernel_setup(void) {
    enter_protected_mode(&_gdt_gdtr);
    pic_remap();
    initialize_idt();
    initialize_memory();
    initialize_vga();
    activate_keyboard_interrupt();
    graphics_clear();
    graphics_cursor_on();
    initialize_filesystem_fat32();
    keyboard_state_activate();

    struct ClusterBuffer cbuf[5];
    for (uint32_t i = 0; i < 5; i++)
        for (uint32_t j = 0; j < CLUSTER_SIZE; j++)
            cbuf[i].buf[j] = i + 'a';

    struct FAT32DriverRequest request = {
        .buf                   = cbuf,
        .name                  = "ikanaide",
        .ext                   = "uwu",
        .parent_cluster_number = ROOT_CLUSTER_NUMBER,
        .buffer_size           = 0,
    } ;

    write(request);  // Create folder "ikanaide"
    memcpy(request.name, "kano1   ", 8);
    write(request);  // Create folder "kano1"
    memcpy(request.name, "ikanaide", 8);
    delete(request); // Delete first folder, thus creating hole in FS
    memcpy(request.name, "daijoubu", 8);
    request.buffer_size = 5*CLUSTER_SIZE;
    write(request);  // Create fragmented file "daijoubu"

    struct ClusterBuffer readcbuf;
    
    read_clusters(&readcbuf, ROOT_CLUSTER_NUMBER+1, 1); 
    // If read properly, readcbuf should filled with 'a'

    request.buffer_size = CLUSTER_SIZE;
    read(request);   // Failed read due not enough buffer size
    request.buffer_size = 5*CLUSTER_SIZE;
    ClusterBuffer* reader = read(request);   // Success read on file "daijoubu"
    close(reader);

    init_shell();
    
    while (TRUE);
}