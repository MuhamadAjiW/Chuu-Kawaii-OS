#include "../lib-header/stdtype.h"
#include "../lib-header/user-shell.h"
#include "../lib-header/syscall.h"
#include "../lib-header/stdlib.h"
#include "../lib-header/stdio.h"
#include "../lib-header/string.h"
#include "../lib-header/commands.h"

#include "../lib-header/chuupad.h"

static text_reader reader = {
    .text_buffer = 0,
    .buffersize = 0,
    .minIdx = 0,
    .currentIdx = 0,
    .len = 0,
};

void initialize_reader(){
    reader.buffersize = CLUSTER_SIZE;
    reader.text_buffer = (char*) malloc (sizeof(char) * CLUSTER_SIZE + 1);
    reader.text_buffer[reader.buffersize] = 0;
}

void close_reader(){
    reader.buffersize = CLUSTER_SIZE;
    free(reader.text_buffer);
    reader.text_buffer[0] = 0;
    reader.currentIdx = 0;
    reader.len = 0;
}


void append_reader(char in){
    if(reader.len < reader.buffersize - 1){
        for(uint32_t i = reader.len; i > reader.currentIdx; i--){
            reader.text_buffer[i] = reader.text_buffer[i-1];
        }
        reader.text_buffer[reader.currentIdx] = in;
        reader.len++;
        reader.currentIdx++;
    }
    else{
        reader.buffersize += CLUSTER_SIZE;
        reader.text_buffer = (char*) realloc(reader.text_buffer, sizeof(char) * reader.buffersize + 1);
        reader.text_buffer[reader.buffersize] = 0;
        append_reader(in);
    }
}
void move_reader(int direction){
    reader.currentIdx = reader.currentIdx + direction;
    return;
}

void backspace_reader(){
    for(uint32_t i = reader.currentIdx-1; i < reader.len-1; i++){
        reader.text_buffer[i] = reader.text_buffer[i+1];
    }
    if(reader.currentIdx > 0){
        reader.currentIdx--;
    }
    if(reader.len > 0){
        reader.len--;
    }

    return;
}

void display_text(char* text){
    syscall(SYSCALL_DISPLAY_TEXT, (uint32_t) text, 0, 0);
}



uint8_t running = 1;

void reader_main(){
    syscall(SYSCALL_LIMIT_CURSOR, 0, 0, 0);    

    initialize_reader();    

    char buf[2] = {0, 0};
    char screen_buffer[25*64 + 1] = {0};
    while (running) {
        buf[0] = getc();
        
        if(buf[0] == TAB_CHAR){
            append_reader(' ');
            append_reader(' ');
            append_reader(' ');
            append_reader(' ');
        }
        else if(buf[0] == LARROW_CHAR){
            if(move_cursor(-1)){
                move_reader(-1);
            }
        }
        
        else if(buf[0] == RARROW_CHAR){
            if(move_cursor(1)){
                move_reader(1);
            }
        }
        if (buf[0] == BACKSPACE_CHAR){
            if(backspace()){
                backspace_reader();
            }
        }
                
        else if((buf[0] >= 32 && buf[0] <= 126) || buf[0] == '\n'){
            append_reader(buf[0]);
            for(uint16_t i = 0; i < 25*64; i++){
                screen_buffer[i] = reader.text_buffer[i + reader.minIdx];
            }
            display_text(screen_buffer);
            if(buf[0] == '\n'){
                move_cursor(2);
            }
            else move_cursor(1);

            
        }
    }
    
    close_reader();
}
