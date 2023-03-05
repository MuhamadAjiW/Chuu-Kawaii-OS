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
#include "lib-header/memory_manager.h"

#include "lib-header/shell.h"


uint32_t target[128] = {0};
uint32_t entry[512];
char buffer[128]; 
ClusterBuffer read_memory[10];

void kernel_setup(void) {
    enter_protected_mode(&_gdt_gdtr);
    pic_remap();
    initialize_idt();
    activate_keyboard_interrupt();
    framebuffer_clear();
    framebuffer_set_cursor(0, 0);
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
    read(request);   // Success read on file "daijoubu"

    init_shell();
    
    while (TRUE);
    /*
    enter_protected_mode(&_gdt_gdtr);
    pic_remap();
    initialize_idt();

    activate_keyboard_interrupt();
    activate_timer_interrupt(20); // Harus > 18, kalo gak rada unpredictable
    
    framebuffer_clear();
    
    for(int i = 0; i < 128; i++){
        target[i] = 0;
    }

    initialize_filesystem_fat32();
    
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
    request.parent_cluster_number = 3;
    write(request);  // Create folder "kano1"

    framebuffer_clear();
    read_blocks(target, cluster_to_lba(1), 1);
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

    memcpy(request.name, "ikanaide", 8);
    request.parent_cluster_number = 2;
    delete(request); // Delete first folder, thus creating hole in FS
    
    framebuffer_clear();
    read_blocks(target, cluster_to_lba(1), 1);
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


    memcpy(request.name, "daijoubu", 8);
    request.buffer_size = 5*CLUSTER_SIZE;
    write(request);



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


    struct ClusterBuffer readcbuf;
    read_clusters(&readcbuf, ROOT_CLUSTER_NUMBER+1, 1); 
    // If read properly, readcbuf should filled with 'a'

    framebuffer_clear();
    for(int i = 0; i < 128; i++){
        int_toString((cbuf->buf[i]), buffer);
        framebuffer_printDef(buffer);
        framebuffer_printDef(" ");
    }

    request.buffer_size = CLUSTER_SIZE;
    read(request);   // Failed read due not enough buffer size
    //request.buffer_size = 5*CLUSTER_SIZE;
    //read(request);   // Success read on file "daijoubu"


    framebuffer_clear();
    keyboard_state_activate();
    init_shell();
    
    while (TRUE);
    */
}