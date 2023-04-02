#include "../lib-header/isr.h"
#include "../lib-header/syscall.h"

#include "../lib-header/stdmem.h"

#include "../lib-header/string.h"
#include "../lib-header/memory_manager.h"

#include "../lib-header/graphics.h"
#include "../lib-header/keyboard.h"
#include "../lib-header/fat32.h"


InterruptService syscall_handler[SYSCALL_COUNT];

void register_syscall_response(uint8_t no, InterruptService response){
    syscall_handler[no] = response;
}

/*syscall functions*/
void idle(){
    return;
}
void sys_malloc(registers r){
    *(void**) r.edx = (void*) malloc(sizeof(r.ebx));
}
void sys_realloc(registers r){
    *(void**) r.edx = (void*) realloc((void*) r.ebx, sizeof(r.ecx));
}
void sys_free(registers r){
    free((void*)r.ebx);
}
void sys_get_last_key(registers r){
    keyboard_state_deactivate();
    *(char*)r.ebx = get_keyboard_last_key();
    keyboard_state_activate();
}
void sys_print_str(registers r){
    graphics_print((char*) r.ebx);
}
void sys_move_cursor(registers r){
    *(uint8_t*)r.ecx = graphics_move_cursor(r.ebx);
}
void sys_backspace(registers r){
    *(uint8_t*)r.ebx = graphics_backspace();
}
void sys_read(registers r){
    struct FAT32DriverRequest request = *(struct FAT32DriverRequest*) r.ebx;
    *((int8_t*) r.ecx) =  load(request);
}
void sys_graphics_clear(){
    graphics_clear();
}

void enable_system_calls(){
    register_interrupt_handler(0x30, syscall);

    register_syscall_response(SYSCALL_IDLE, idle);
    register_syscall_response(SYSCALL_MALLOC, sys_malloc);
    register_syscall_response(SYSCALL_REALLOC, sys_realloc);
    register_syscall_response(SYSCALL_FREE, sys_free);
    register_syscall_response(SYSCALL_GET_KEYBOARD_LAST_KEY, sys_get_last_key);
    register_syscall_response(SYSCALL_PRINT_STR, sys_print_str);
    register_syscall_response(SYSCALL_CLEAR_SCREEN, sys_graphics_clear);
    register_syscall_response(SYSCALL_MOVE_CURSOR, sys_move_cursor);
    register_syscall_response(SYSCALL_BACKSPACE, sys_backspace);
    register_syscall_response(SYSCALL_READ_FILE, sys_read);
}

void syscall(registers r) {
    InterruptService handler = syscall_handler[r.eax];
    handler(r);
}
