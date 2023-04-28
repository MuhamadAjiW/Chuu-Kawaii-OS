#include "lib-header/portio.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/gdt.h"
#include "lib-header/kernel_loader.h"
#include "lib-header/idt.h"
#include "lib-header/isr.h"
#include "lib-header/pit.h"
#include "lib-header/keyboard.h"
#include "lib-header/string.h"
#include "lib-header/fat32.h"
#include "lib-header/disk.h"
#include "lib-header/memory_manager.h"
#include "lib-header/graphics.h"
#include "lib-header/cmos.h"
#include "lib-header/paging.h"
#include "lib-header/tss.h"
#include "lib-header/syscall.h"

//debug purposes
//uint32_t target[CLUSTER_SIZE/4] = {0};
//uint32_t entry[512];
//char buffer[128]; 
//DirectoryTable table;
//cmos_reader tester;

void kernel_setup(void) {


    enter_protected_mode(&_gdt_gdtr);
    pic_remap();
    initialize_idt();
    
    initialize_memory();
    
    initialize_vga();

    activate_keyboard_interrupt();
    graphics_clear_buffer();
    graphics_cursor_on();
    initialize_filesystem_fat32();
    
    
    graphics_display_animation();
    
    gdt_install_tss();
    set_tss_register();

    allocate_single_user_page_frame(0);

    struct FAT32DriverRequest request = {
        .buf                   = (uint8_t*)0,
        .name                  = "sh",
        .ext                   = "\0\0\0",
        .parent_cluster_number = ROOT_CLUSTER_NUMBER,
        .buffer_size           = 0x100000,
    };
    load(request);

    set_tss_kernel_current_stack();
    
    enable_system_calls();

    graphics_print_color("Hello from kernel\n", 84);
    kernel_execute_user_program((uint8_t*)0);
    
    
    /*
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

    memcpy(request.name, "kano2   ", 8);
    request.parent_cluster_number = 6;
    write(request);


    
    struct ClusterBuffer readcbuf;
    
    read_clusters(&readcbuf, ROOT_CLUSTER_NUMBER+1, 1); 
    // If read properly, readcbuf should filled with 'a'


    
    request.buffer_size = CLUSTER_SIZE;
    read(request);   // Failed read due not enough buffer size
    request.buffer_size = 5*CLUSTER_SIZE;
    ClusterBuffer* reader = read(request);   // Success read on file "daijoubu"
    close(reader);

    
    */
    while (TRUE);
}
    /*
    //testing untuk extendable folder
    for(int i = 10; i < 80; i++){
        int_toString(i, buffer);
        buffer[2] = ' ';
        buffer[3] = ' ';
        buffer[4] = ' ';
        buffer[5] = ' ';
        buffer[6] = ' ';
        buffer[7] = ' ';
        memcpy(request.name, buffer, 8);
        write(request);
    }
    */