#include "lib-header/keyboard.h"
#include "lib-header/framebuffer.h"
#include "lib-header/stdtype.h"
#include "lib-header/string.h"
#include "lib-header/shell.h"



void init_shell(){
    clear_reader();
    framebuffer_printDef("\n    >> ");
    framebuffer_set_cursor(1,7);
    framebuffer_set_limit(1,6);
}

void execute_reader(){
    if(strcmp(get_keyboard_buffer(), "clear") == 0){
        framebuffer_clear();
        clear_reader();
    }
    else{
        framebuffer_printDef("\nYou wrote: ");
        framebuffer_printDef(get_keyboard_buffer());
        framebuffer_printDef("\n");
    }
}

/*
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
*/

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

