#include "drivers/keyboard.h"
#include "lib-header/framebuffer.h"
#include "lib-header/stdtype.h"

char shellReader[80*24];
int maxIdx = 0;
int currentIdx = 0;

void init_shell(){
    init_keyboard();
    framebuffer_printDef("    >> ");
    framebuffer_set_cursor(0,7);
    framebuffer_set_limit(0,6);
}

void clear_reader(){
    shellReader[0] = 0;
    currentIdx = 0;
    maxIdx = 0;
}

void execute_reader(){
    framebuffer_printDef("\nYou wrote: ");
    framebuffer_printDef(shellReader);
    framebuffer_printDef("\n");
}

bool closed_sentence(){
    bool closed = 0;

    for(int i = maxIdx-1; i >= 0; i--){
        if(shellReader[i] == ' ' || shellReader[i] == '\n'){}
        else if(shellReader[i] == ';'){
            shellReader[i] = 0;
            return 1;
        }
        else{
            return 0;
        }
    }

    return closed;
}

void newline_shell(){
    uint16_t cursor = framebuffer_get_cursor();
    uint8_t row = cursor/80;

    if(row != 24){
        framebuffer_set_cursor(row + 1, 0);
        framebuffer_printDef("    >> ");
        framebuffer_set_limit(row + 1,6);
        framebuffer_set_cursor(row + 1, 7);
    }
    else{
        framebuffer_set_cursor(row, 0);
        framebuffer_scroll();
        framebuffer_printDef("    >> ");
        framebuffer_set_limit(row,6);
        framebuffer_set_cursor(row, 7);
    }
}

void append_reader(char in){
    if(!(in == '\n' && closed_sentence())){
        shellReader[currentIdx] = in;
        if(currentIdx == maxIdx){
            maxIdx++;
        }
        currentIdx++;
    }
    else{
        shellReader[currentIdx] = 0;
        execute_reader();
        clear_reader();
    }
}

void move_reader(int direction){
    currentIdx = currentIdx + direction;
}

void backspace_reader(){
    for(int i = currentIdx-1; i < maxIdx-1; i++){
        shellReader[i] = shellReader[i+1];
    }
    currentIdx--;
    maxIdx--;
}

