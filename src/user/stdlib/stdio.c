
#include "../lib-header/stdio.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/syscall.h"

void print(char* string){
    syscall(SYSCALL_PRINT_STR, (uint32_t) string, 0, 0);
}

void print_char(char c){
    syscall(SYSCALL_PRINT_CHAR, (uint32_t) c, 0, 0);
}

void print_color(char* string, uint8_t color){
    syscall(SYSCALL_PRINT_STR_C, (uint32_t) string, (uint32_t) color, 0);
}
void print_char_color(char c, uint8_t color){
    syscall(SYSCALL_PRINT_CHAR_C, (uint32_t) c, (uint32_t) color, 0);
}

char getc(){
    char c;
    syscall(SYSCALL_GET_KEYBOARD_LAST_KEY, (uint32_t) &c, 0, 0);
    return c;
}

FAT32FileReader readf(FAT32DriverRequest request){
    FAT32FileReader retval;
    syscall(SYSCALL_READ_FILE, (uint32_t) &request, (uint32_t) &retval, 0);
    return retval;
}

FAT32DirectoryReader readf_dir(FAT32DriverRequest request){
    FAT32DirectoryReader retval;
    syscall(SYSCALL_READ_DIR, (uint32_t) &request, (uint32_t) &retval, 0);
    return retval;
}

void closef(FAT32FileReader request){
    syscall(SYSCALL_CLOSE_FILE, (uint32_t) &request, 0, 0);
}

void closef_dir(FAT32DirectoryReader request){
    syscall(SYSCALL_CLOSE_DIR, (uint32_t) &request, 0, 0);
}

uint8_t writef(FAT32DriverRequest request){
    uint8_t status;
    syscall(SYSCALL_WRITE_FILE, (uint32_t) &request, (uint32_t) &status, 0);
    return status;
}

uint8_t deletef(FAT32DriverRequest request){
    uint8_t status;
    syscall(SYSCALL_DELETE_FILE, (uint32_t) &request, (uint32_t) &status, 0);
    return status;
}

DirectoryTable asdirectory(uint32_t* reader){
    DirectoryTable table;
    syscall(SYSCALL_AS_DIRECTORY, (uint32_t) reader, (uint32_t) &table, 0);
    return table;
}

uint8_t isdirectory(uint32_t cluster){
    uint8_t isTrue;
    syscall(SYSCALL_IS_DIRECTORY, (uint32_t) cluster, (uint32_t) &isTrue, 0);
    return isTrue;
}
