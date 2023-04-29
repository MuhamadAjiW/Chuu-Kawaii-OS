#include "lib-header/stdtype.h"
#include "lib-header/user-shell.h"
#include "lib-header/syscall.h"
#include "lib-header/stdlib.h"
#include "lib-header/stdio.h"
#include "lib-header/stdmem.h"
#include "lib-header/string.h"
#include "lib-header/commands.h"
#include "lib-header/chuupad.h"
#include "lib-header/commands-util.h"

#include "lib-header/parser.h"

static shell_reader shell = {
    .keyboard_buffer = (char*) 1,
    .buffersize = INPUT_BUFFER_SIZE,
    .maxIdx = 0,
    .currentIdx = 0
};

static directory_info current_dir = {
    .directory_path = (char*) 0,
    .cluster_number = 0
};

void initialize_shell(){
    current_dir.cluster_number = 2;
    current_dir.directory_path = (char*) malloc (sizeof(char) * INPUT_BUFFER_SIZE);
    memcpy(current_dir.directory_path, "/root", 6);
    shell.buffersize = INPUT_BUFFER_SIZE;
    shell.keyboard_buffer = (char*) malloc (sizeof(char) * INPUT_BUFFER_SIZE);
    newline_shell();
}

void close_shell(){
    shell.buffersize = 0;
    free(shell.keyboard_buffer);
    free(current_dir.directory_path);
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
    print("\n");
    print(current_dir.directory_path);
    print("> ");
    syscall(SYSCALL_LIMIT_CURSOR, 0, 0, 0);
    return;
}



// uint32_t currentCluster = 2;



#define MAX_RESULTS 100 


void evaluate_shell(){
    parse(shell.keyboard_buffer);
    // parse_path(shell.keyboard_buffer);
    if (get_parsed_word_count() > 0){
        if(strcmp(get_parsed_result()[0], "clear") == 0){
            clear();
        }
        else if(strcmp(get_parsed_result()[0], "chuu") == 0){
            animation();
        }
        else if(strcmp(get_parsed_result()[0], "dir") == 0){
            dir(current_dir.cluster_number);
        }
        else if(strcmp(get_parsed_result()[0], "cd") == 0){
            // current_dir = cd(get_parsed_result()[1], current_dir);
            if (get_parsed_word_count() == 1){
                current_dir.cluster_number = 2;
                memcpy(current_dir.directory_path, "/root", 6);
            }
            else if (is_directorypath_valid(get_parsed_result()[1], current_dir.cluster_number)){
                cd(get_parsed_result()[1], &current_dir);
            } else {
                print("\ncd: no such directory: ");
                print(get_parsed_result()[1]);
                print("\n");
            }
        }
        else if(strcmp(get_parsed_result()[0], "ls") == 0){
            if (get_parsed_word_count() == 1){
                ls(current_dir.cluster_number);
            }
            else if (is_directorypath_valid(get_parsed_result()[1], current_dir.cluster_number)){
                ls(path_to_cluster(get_parsed_result()[1], current_dir.cluster_number));
            } else {
                print("\nls: ");
                print(get_parsed_result()[1]);
                print(": No such file or directory\n");
            }  
        }
        else if(strcmp(get_parsed_result()[0], "cat") == 0){
            if (get_parsed_word_count() == 1){
                // masuk state mini program
            } else if (is_filepath_valid(get_parsed_result()[1], current_dir.cluster_number)){
                cat(current_dir.cluster_number);
            } else if (is_directorypath_valid(get_parsed_result()[1], current_dir.cluster_number)){
                print("\ncat: ");
                print(get_parsed_result()[1]);
                print(": Is a directory\n");
            } else {
                print("\ncat: ");
                print(get_parsed_result()[1]);
                print(": No such file\n");
            }
        }
        else if (strcmp(get_parsed_result()[0], "rm") == 0) {
            rm(current_dir.cluster_number);

        } else if (strcmp(get_parsed_result()[0], "cp") == 0) {
            cp(current_dir.cluster_number);
        } else if (strcmp(get_parsed_result()[0], "mv") == 0) {
            mv(current_dir.cluster_number);
        }
        else if(strcmp(get_parsed_result()[0], "chuupad") == 0){
            if(get_parsed_word_count() == 1){
                clear();
                reader_no_file();
                clear();
            }
            else if (get_parsed_word_count() == 2){
                if (!is_filepath_valid(get_parsed_result()[1], current_dir.cluster_number)){
                    print("\nchuupad: ");
                    print(get_parsed_result()[1]);
                    print(": No such file\n");
                }
                else{
                    clear();
                    reader_with_file(current_dir.cluster_number);
                    clear();
                }
            }
            else{
                print("\nchuupad: Invalid command\n");
            }

        }
        else if(strcmp(get_parsed_result()[0], "mkdir") == 0){
            if(get_parsed_word_count() != 2){
                print("\nmkdir: Invalid command\n");
            }
            else{
                mkdir(get_parsed_result()[1], current_dir.cluster_number);
            }
        }  
        else if (strcmp(get_parsed_result()[0], "whereis") == 0) {
            whereis(current_dir.cluster_number, get_parsed_result()[1], current_dir.directory_path);
        }

        // check result array
        else{
            print("\nNo Command found: ");
            print(shell.keyboard_buffer);
            print("\n");
        }
    }    
    parser_clear(); 
    
}

int main(void){
    char* hello = "hewwo you are in user mode UWU\n";
    print(hello);

    initialize_shell();

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

    struct FAT32DriverRequest file = {
        .buf                   = cbuf,
        .name                  = "babibabi",
        .ext                   = "uwu",
        .parent_cluster_number = ROOT_CLUSTER_NUMBER,
        .buffer_size           = CLUSTER_SIZE,
    } ;

    // request.buffer_size = CLUSTER_SIZE;
    writef(request);  // Create folder "ikanaide"
    //deletef(request); // Delete first folder, thus creating hole in FS
    
    writef(file);  // Create fragmented file "daijoubu"
    
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
            print_color(buf, 84);
        }
    }
    
    close_shell();
    
    return 0;
}

