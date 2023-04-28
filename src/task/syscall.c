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

//TODO: assert stability
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

void sys_read_clusters(registers r){
   read_clusters((void*) r.ebx, (uint16_t) r.ecx, (uint16_t) r.edx);
}

void sys_as_directory(registers r){
   *((DirectoryTable*) r.ecx) = as_directory((uint32_t*) r.ebx);
}

void sys_is_directory(registers r){
   *((uint8_t*) r.ecx) = is_directory((uint32_t)r.ebx);

void sys_mkdir(registers r){
    struct FAT32DriverRequest request = *(struct FAT32DriverRequest*) r.ebx;
    *((int8_t*) r.ecx) = write(request);
}

// void sys_where(registers r){
//     uint16_t cluster_number = *((uint16_t*) r.ebx);
//     FAT32DriverRequest* result_array = (FAT32DriverRequest*) r.ecx;
//     uint16_t* result_count = (uint16_t*) r.edx;
//     whereis(cluster_number, result_array, result_count);
// }

void sys_memcpy(registers r) {
    void* dest = (void*)r.ebx;
    const void* src = (const void*)r.ecx;
    uint32_t n = (uint32_t)r.edx;

    memcpy(dest, src, n);
}

void sys_memcmp(registers r) {
    const void* s1 = (const void*)r.ebx;
    const void* s2 = (const void*)r.ecx;
    uint32_t n = (uint32_t)r.edx;

    int result = memcmp(s1, s2, n);

    r.eax = result;
}


void sys_read_clusters(registers r){
    void* reader = (void*) r.ebx;
    uint16_t cluster = ( (uint16_t) r.ecx);
    uint16_t sector_count = ((uint16_t) r.edx);
    read_clusters(reader, cluster, sector_count);
}

void sys_as_directory(registers r){
    uint32_t* reader = (uint32_t*) r.ebx;
    DirectoryTable table = as_directory(reader);
    memcpy((void*)r.ecx, &table, sizeof(DirectoryTable));
}

void sys_name_exist(registers r){
    FAT32DriverRequest request = *(FAT32DriverRequest*) r.ebx;
    *((int8_t*) r.ecx) = name_exists(request);
}

void enable_system_calls(){
    register_interrupt_handler(0x30, syscall_response);

    register_syscall_response(SYSCALL_IDLE, idle);
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
    register_syscall_response(SYSCALL_CLOSE_FILE, sys_close_file);
    register_syscall_response(SYSCALL_CLOSE_DIR, sys_close_directory);
    register_syscall_response(SYSCALL_WRITE_FILE, sys_write);
    register_syscall_response(SYSCALL_DELETE_FILE, sys_delete);
    register_syscall_response(SYSCALL_GET_CMOS_DATA, sys_get_cmos_data);
    register_syscall_response(SYSCALL_ANIMATION, sys_play_animation);
    register_syscall_response(SYSCALL_GETTICK, sys_get_timer_tick);
    register_syscall_response(SYSCALL_READ_CLUSTERS, sys_read_clusters);
    register_syscall_response(SYSCALL_AS_DIRECTORY, sys_as_directory);
    register_syscall_response(SYSCALL_IS_DIRECTORY, sys_is_directory);
    register_syscall_response(SYSCALL_MKDIR, sys_mkdir);
    // register_syscall_response(SYSCALL_WHERE, sys_where);
    register_syscall_response(SYSCALL_MEMCPY, sys_memcpy);
    register_syscall_response(SYSCALL_MEMCMP, sys_memcmp);
    register_syscall_response(SYSCALL_READ_CLUSTERS, sys_read_clusters);
    register_syscall_response(SYSCALL_AS_DIRECTORY, sys_as_directory);
    register_syscall_response(SYSCALL_NAME_EXISTS, sys_name_exist);
}

void syscall_response(registers r) {
    InterruptService handler = syscall_handler[r.eax];
    handler(r);
}

