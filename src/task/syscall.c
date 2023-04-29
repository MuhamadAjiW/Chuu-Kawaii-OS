#include "../lib-header/isr.h"
#include "../lib-header/syscall.h"

#include "../lib-header/stdmem.h"

#include "../lib-header/string.h"
#include "../lib-header/memory_manager.h"

#include "../lib-header/graphics.h"
#include "../lib-header/keyboard.h"
#include "../lib-header/cmos.h"
#include "../lib-header/fat32.h"
#include "../lib-header/pit.h"


InterruptService syscall_handler[SYSCALL_COUNT];

void register_syscall_response(uint8_t no, InterruptService response){
    syscall_handler[no] = response;
}

/*syscall functions*/
void idle(){
    return;
}
void sys_malloc(registers r){
    *(void**) r.edx = (void*) kmalloc(r.ebx);
}
void sys_realloc(registers r){
    *(void**) r.edx = (void*) krealloc((void*) r.ebx, r.ecx);
}
void sys_free(registers r){
    kfree((void*)r.ebx);
}
void sys_get_last_key(registers r){
    keyboard_state_deactivate();
    *(char*)r.ebx = get_keyboard_last_key();
    keyboard_state_activate();
}
void sys_print_str(registers r){
    graphics_print((char*) r.ebx);
}
void sys_print_char(registers r){
    graphics_write_char((char) r.ebx);
}

void sys_print_str_color(registers r){
    graphics_print_color((char*) r.ebx, (uint8_t) (r.ecx & 0xff));
}
void sys_print_char_color(registers r){
    graphics_write_char_color((char) r.ebx, (uint8_t) (r.ecx & 0xff));
}

void sys_move_cursor(registers r){
    *(uint8_t*)r.ecx = graphics_move_cursor(r.ebx);
}
void sys_backspace(registers r){
    *(uint8_t*)r.ebx = graphics_backspace();
}
void sys_graphics_clear(){
    graphics_clear_buffer();
}
void sys_graphics_limit_cursor(){
    graphics_set_limit(graphics_get_cursor_x(), graphics_get_cursor_y());
}
void sys_delete(registers r){
    struct FAT32DriverRequest request = *(struct FAT32DriverRequest*) r.ebx;
    *((int8_t*) r.ecx) =  delete(request);
}
void sys_write(registers r){
    struct FAT32DriverRequest request = *(struct FAT32DriverRequest*) r.ebx;
    *((int8_t*) r.ecx) =  write(request);
}
void sys_read(registers r){
    struct FAT32DriverRequest request = *(struct FAT32DriverRequest*) r.ebx;
    *((FAT32FileReader*) r.ecx) = read(request);
}
void sys_read_directory(registers r){
    struct FAT32DriverRequest request = *(struct FAT32DriverRequest*) r.ebx;
    *((FAT32DirectoryReader*) r.ecx) = read_directory(request);
}
void sys_self_directory_info(registers r){
    *((FAT32DirectoryReader*) r.ecx) = self_directory_info(r.ebx);
}
void sys_close_file(registers r){
    close_file(*(struct FAT32FileReader*) r.ebx);
}
void sys_close_directory(registers r){
    close_directory(*(struct FAT32DirectoryReader*) r.ebx);
}
void sys_get_cmos_data(registers r){
    *((cmos_reader*) r.ebx) = get_cmos_data();
}
void sys_play_animation(registers r){
    graphics_user_animation(r.ebx);
}
void sys_get_timer_tick(registers r){
    *(uint32_t*)r.ebx = get_tick();
}

void sys_as_directory(registers r){
   *((DirectoryTable*) r.ecx) = as_directory((uint32_t*) r.ebx);
}

void sys_is_directory(registers r){
   *((uint8_t*) r.ecx) = is_directory((uint32_t)r.ebx);
}

void sys_name_exist(registers r){
    FAT32DriverRequest request = *(FAT32DriverRequest*) r.ebx;
    *((int8_t*) r.ecx) = name_exists(request);
}

void sys_display_text(registers r){
    graphics_display_text((char*) r.ebx);
}
void sys_graphics_get_cursor_y(registers r){
    *(uint8_t*) r.ebx = graphics_get_cursor_y();
}
void sys_graphics_get_cursor_x(registers r){
    *(uint8_t*) r.ebx = graphics_get_cursor_x();
}
void sys_graphics_get_text_edge(registers r){
    *(uint8_t*) r.ecx = graphics_find_edge(r.ebx);
}

void enable_system_calls(){
    register_interrupt_handler(0x30, syscall_response);

    register_syscall_response(SYSCALL_IDLE, idle);
    register_syscall_response(SYSCALL_ANIMATION, sys_play_animation);
    register_syscall_response(SYSCALL_MALLOC, sys_malloc);
    register_syscall_response(SYSCALL_REALLOC, sys_realloc);
    register_syscall_response(SYSCALL_FREE, sys_free);
    register_syscall_response(SYSCALL_GET_KEYBOARD_LAST_KEY, sys_get_last_key);
    register_syscall_response(SYSCALL_PRINT_STR, sys_print_str);
    register_syscall_response(SYSCALL_PRINT_STR_C, sys_print_str_color);
    register_syscall_response(SYSCALL_PRINT_CHAR, sys_print_char);
    register_syscall_response(SYSCALL_PRINT_CHAR_C, sys_print_char_color);
    register_syscall_response(SYSCALL_CLEAR_SCREEN, sys_graphics_clear);
    register_syscall_response(SYSCALL_MOVE_CURSOR, sys_move_cursor);
    register_syscall_response(SYSCALL_LIMIT_CURSOR, sys_graphics_limit_cursor);
    register_syscall_response(SYSCALL_BACKSPACE, sys_backspace);
    register_syscall_response(SYSCALL_READ_FILE, sys_read);
    register_syscall_response(SYSCALL_READ_DIR, sys_read_directory);
    register_syscall_response(SYSCALL_SELF_DIR_INFO, sys_self_directory_info);
    register_syscall_response(SYSCALL_WRITE_FILE, sys_write);
    register_syscall_response(SYSCALL_DELETE_FILE, sys_delete);
    register_syscall_response(SYSCALL_CLOSE_FILE, sys_close_file);
    register_syscall_response(SYSCALL_CLOSE_DIR, sys_close_directory);
    register_syscall_response(SYSCALL_GET_CMOS_DATA, sys_get_cmos_data);
    register_syscall_response(SYSCALL_GETTICK, sys_get_timer_tick);
    register_syscall_response(SYSCALL_AS_DIRECTORY, sys_as_directory);
    register_syscall_response(SYSCALL_IS_DIRECTORY, sys_is_directory);
    register_syscall_response(SYSCALL_NAME_EXISTS, sys_name_exist);

    register_syscall_response(SYSCALL_DISPLAY_TEXT, sys_display_text);
    register_syscall_response(SYSCALL_GET_CURSOR_Y, sys_graphics_get_cursor_y);
    register_syscall_response(SYSCALL_GET_CURSOR_X, sys_graphics_get_cursor_x);
    register_syscall_response(SYSCALL_GET_TEXT_EDGE, sys_graphics_get_text_edge);
}

void syscall_response(registers r) {
    InterruptService handler = syscall_handler[r.eax];
    handler(r);
}

