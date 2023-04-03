#include "lib-header/stdtype.h"
#include "lib-header/user-shell.h"
#include "lib-header/syscall.h"
#include "lib-header/stdlib.h"
#include "lib-header/stdio.h"
#include "lib-header/string.h"
#include "lib-header/commands.h"

#include "lib-header/parser.h"

static shell_reader shell = {
    .keyboard_buffer = (char*) 1,
    .buffersize = INPUT_BUFFER_SIZE,
    .maxIdx = 0,
    .currentIdx = 0
};

void initialize_shell(){
    shell.buffersize = INPUT_BUFFER_SIZE;
    shell.keyboard_buffer = (char*) malloc (sizeof(char) * INPUT_BUFFER_SIZE);
    newline_shell();
}

void close_shell(){
    shell.buffersize = 0;
    free(shell.keyboard_buffer);
}

void clear_shell(){
    shell.buffersize = INPUT_BUFFER_SIZE;
    free(shell.keyboard_buffer);
    shell.keyboard_buffer = (char*) malloc (INPUT_BUFFER_SIZE);;
    shell.keyboard_buffer[0] = 0;
    shell.currentIdx = 0;
    shell.maxIdx = 0;
}


void append_shell(char in){
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
            evaluate_shell();
            clear_shell();
            newline_shell();
        }
    }
    else{
        shell.buffersize += INPUT_BUFFER_SIZE;
        shell.keyboard_buffer = (char*) realloc (shell.keyboard_buffer, sizeof(char) * shell.buffersize);
        append_shell(in);
    }
}
void move_shell(int direction){
    shell.currentIdx = shell.currentIdx + direction;
    return;
}

void backspace_shell(){
    for(uint32_t i = shell.currentIdx-1; i < shell.maxIdx-1; i++){
        shell.keyboard_buffer[i] = shell.keyboard_buffer[i+1];
    }
    if(shell.currentIdx > 0){
        shell.currentIdx--;
    }
    if(shell.maxIdx > 0){
        shell.maxIdx--;
    }

    return;
}

char* get_keyboard_buffer(){
    return shell.keyboard_buffer;
}

void newline_shell(){
    print("\n    >> ");
    syscall(SYSCALL_LIMIT_CURSOR, 0, 0, 0);
    return;
}








char currentDir[8] = {'r', 'o', 'o', 't',' ', ' ', ' ', ' '};
uint32_t currentCluster = 2;

void evaluate_shell(){
    parse(shell.keyboard_buffer);
    if (get_parsed_word_count() > 0){
        if(strcmp(get_parsed_result()[0], "clear") == 0){
            clear();
        }
        else{
            print("\nNo Command found: ");
            print(shell.keyboard_buffer);
            print("\n");
        }
    }    
    parser_clear();    
}

int main(void){
    char* hello = "hewwo\n";
    print(hello);

    initialize_shell();

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
    
    char buf[2] = {0, 0};
    while (TRUE) {
        buf[0] = getc();
        
        if(buf[0] == TAB_CHAR){
            append_shell(' ');
            append_shell(' ');
            append_shell(' ');
            append_shell(' ');
            print("    ");
        }
        else if(buf[0] == LARROW_CHAR){
            if(move_cursor(-1)){
                move_shell(-1);
            }
        }
        
        else if(buf[0] == RARROW_CHAR){
            if(move_cursor(1)){
                move_shell(1);
            }
        }
        if (buf[0] == BACKSPACE_CHAR){
            if(backspace()){
                backspace_shell();
            }
        }
        
        if(buf[0] == '\n'){
            append_shell('\n');
        }  
        
        else if(buf[0] >= 32 && buf[0] <= 126){
            append_shell(buf[0]);
            print(buf);
        }
    }
    
    close_shell();
    
    return 0;
}
