#include "lib-header/stdtype.h"
#include "lib-header/user-shell.h"
#include "lib-header/syscall.h"
#include "lib-header/stdmem.h"
#include "lib-header/string.h"
#include "lib-header/parser.h"

static shell_reader shell = {
    .keyboard_buffer = 0,
    .buffersize = 0,
    .maxIdx = 0,
    .currentIdx = 0
};

void init_shell(){
    shell.keyboard_buffer = (char*) malloc (sizeof(INPUT_BUFFER_SIZE));
    shell.buffersize = INPUT_BUFFER_SIZE;
}

void close_shell(){
    free(shell.keyboard_buffer);
}

void clear_reader(){
    shell.buffersize = INPUT_BUFFER_SIZE;
    free(shell.keyboard_buffer);
    shell.keyboard_buffer = (char*) malloc (INPUT_BUFFER_SIZE);;
    shell.keyboard_buffer[0] = 0;
    shell.currentIdx = 0;
    shell.maxIdx = 0;
}

void append_reader(char in){
    if(shell.maxIdx < shell.buffersize - 1){
        if(!(in == '\n')){
            for(uint32_t i = shell.maxIdx; i > shell.currentIdx; i--){
                shell.keyboard_buffer[i] = shell.keyboard_buffer[i-1];
            }

            shell.keyboard_buffer[shell.currentIdx] = in;
            shell.maxIdx++;
            shell.currentIdx++;
        }
        else{
            shell.keyboard_buffer[shell.maxIdx] = 0;
            //execute_reader();
            clear_reader();
        }
    }
    else{
        shell.buffersize += INPUT_BUFFER_SIZE;
        shell.keyboard_buffer = (char*) realloc (shell.keyboard_buffer, sizeof(char) * shell.buffersize);
        append_reader(in);
    }
}

void move_reader(int direction){
    shell.currentIdx = shell.currentIdx + direction;
}

void backspace_reader(){
    for(uint32_t i = shell.currentIdx-1; i < shell.maxIdx-1; i++){
        shell.keyboard_buffer[i] = shell.keyboard_buffer[i+1];
    }
    if(shell.currentIdx > 0){
        shell.currentIdx--;
    }
    if(shell.maxIdx > 0){
        shell.maxIdx--;
    }
}

char* get_keyboard_buffer(){
    return shell.keyboard_buffer;
}

void execute(){
    split_words(shell.keyboard_buffer);

    if (get_word_count() > 0){
        if(strcmp(get_parsed()[0], "clear")){
            syscall(SYSCALL_CLEAR_SCREEN, 0, 0, 0);
        }
    }

    clear_parser();
    clear_reader();
}

int main(void) {
    init_shell();

    /*
    struct ClusterBuffer cl           = {0};
    struct FAT32DriverRequest request = {
        .buf                   = &cl,
        .name                  = "ikanaide",
        .ext                   = "\0\0\0",
        .parent_cluster_number = ROOT_CLUSTER_NUMBER,
        .buffer_size           = CLUSTER_SIZE,
    };
    int32_t retcode;
    syscall(SYSCALL_READ_FILE, (uint32_t) &request, (uint32_t) &retcode, 0);
    */

    syscall(SYSCALL_PRINT_STR, (uint32_t) "owo\n", 4, 0xF);
    
    
    char buf = 0;
    while (TRUE) {
        syscall(SYSCALL_GET_KEYBOARD_LAST_KEY, (uint32_t) &buf, 0, 0);
        
        
        if(buf == TAB_CHAR){
            append_reader(' ');
            append_reader(' ');
            append_reader(' ');
            append_reader(' ');
            syscall(SYSCALL_PRINT_STR, (uint32_t) "    ", 0, 0);
        }
        else if(buf == LARROW_CHAR){
            uint8_t success = 0;
            syscall(SYSCALL_MOVE_CURSOR, (uint32_t) -1, (uint32_t) success, 0);
            if(success){
                move_reader(-1);
            }
        }
        
        else if(buf == RARROW_CHAR){
            uint8_t success = 0;
            syscall(SYSCALL_MOVE_CURSOR, (uint32_t) 1, (uint32_t) success, 0);
            if(success){
                move_reader(1);
            }
        }
        else if(buf == BACKSPACE_CHAR){
            uint8_t success = 0;
            syscall(SYSCALL_BACKSPACE, (uint32_t) success, 0, 0);
            if(success){
                move_reader(-1);
            }
        }
        
        else if(buf == '\n'){
            execute();
            syscall(SYSCALL_PRINT_STR, (uint32_t) &buf, 0, 0);
        }
        
        
        else if(buf >= 32 && buf <= 126){
            syscall(SYSCALL_PRINT_STR, (uint32_t) &buf, 0, 0);
        }
        
    }
    
    close_shell();
    return 0;
}
