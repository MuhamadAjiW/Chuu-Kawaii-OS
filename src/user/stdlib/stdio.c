
#include "../lib-header/stdio.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/syscall.h"

void print(char* string){
    syscall(SYSCALL_PRINT_STR, (uint32_t) string, 0, 0);
}

void print_char(char c){
    syscall(SYSCALL_PRINT_CHAR, (uint32_t) c, 0, 0);
}

void loadf(){
    //TODO: Implement
}

void readf(){
    //TODO: Implement
}

void readf_folder(){
    //TODO: Implement
}

void writef(){
    //TODO: Implement
}

void deletef(){
    //TODO: Implement
}

