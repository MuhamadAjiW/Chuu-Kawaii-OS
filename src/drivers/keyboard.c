#include "../lib-header/keyboard.h"
#include "../lib-header/shell.h"
#include "../lib-header/portio.h"
#include "../lib-header/isr.h"
#include "../lib-header/idt.h"
#include "../lib-header/string.h"
#include "../lib-header/graphics.h"

extern void* isr_stub_table[];

static char scantable[64] = {
    0,0,'1','2','3','4','5','6','7','8',
    '9','0','-','=',0,0,'q','w','e','r',
    't','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';',
    '\'',0,0,0,'z','x','c','v','b','n',
    'm',',','.','/',0,0,0,' ',0,0,
    0,0,0,0
};
static char scantableCaps[64] = {
    0,0,'!','@','#','$','%','^','&','*',
    '(',')','_','+',0,0,'Q','W','E','R',
    'T','Y','U','I','O','P','{','}','\n',0,
    'A','S','D','F','G','H','J','K','L',':',
    '"',0,0,0,'Z','X','C','V','B','N',
    'M','<','>','?',0,0,0,' ',0,0,
    0,0,0,0
};

static keyboardDriverState key ={
    .currentIdx = 0,
    .maxIdx = 0,
    .shift = 0,
    .caps = 0,
    .keyboard_input_on = 0
};

static void keyboard_callback(){
    uint8_t scanner = in(0x60);
    if(key.keyboard_input_on){
        keyboard_driver_graphics(scanner);
    }
}

void activate_keyboard_interrupt(){
    register_interrupt_handler(33, keyboard_callback);
}

void keyboard_state_activate(){
    key.keyboard_input_on = 1;
}

void keyboard_state_deactivate(){
    key.keyboard_input_on = 0;
}

void clear_reader(){
    key.keyboard_buffer[0] = 0;
    key.currentIdx = 0;
    key.maxIdx = 0;
}

void append_reader(char in){
    if(key.maxIdx < 500 - 1){
        if(!(in == '\n')){
            for(int i = key.maxIdx; i > key.currentIdx; i--){
                key.keyboard_buffer[i] = key.keyboard_buffer[i-1];
            }

            key.keyboard_buffer[key.currentIdx] = in;
            key.maxIdx++;
            key.currentIdx++;
        }
        else{
            key.keyboard_buffer[key.maxIdx] = 0;
            execute_reader();
            clear_reader();
        }
    }
    else{
        graphics_print("\nBuffer overflowed, resetting buffer...\n");
        append_reader('\n');
        clear_reader();
        newline_shell();
    }
}

void move_reader(int direction){
    key.currentIdx = key.currentIdx + direction;
}

void backspace_reader(){
    for(int i = key.currentIdx-1; i < key.maxIdx-1; i++){
        key.keyboard_buffer[i] = key.keyboard_buffer[i+1];
    }
    if(key.currentIdx > 0){
        key.currentIdx--;
    }
    if(key.maxIdx > 0){
        key.maxIdx--;
    }
}

char* get_keyboard_buffer(){
    return key.keyboard_buffer;
}

void keyboard_driver_graphics(uint8_t input){
        switch (input){
            case 58:
                if(key.caps){
                    key.caps = 0;
                }
                else{
                    key.caps = 1;
                }
                break;
            case 42:
                key.shift = 1;
                break;
            case 170:
                key.shift = 0;
                break;
            case 54:
                key.shift = 1;
                break;
            case 182:
                key.shift = 0;
                break;

            case 14:
                if (graphics_backspace()){
                    backspace_reader();
                }
                break;

            case 15:
                graphics_write_char(' ');
                graphics_write_char(' ');
                graphics_write_char(' ');
                graphics_write_char(' ');
                append_reader(' ');
                append_reader(' ');
                append_reader(' ');
                append_reader(' ');
                break;

            case 75:
                if (graphics_move_cursor(-1)){
                    move_reader(-1);
                };
                break;
            case 77:
                if (graphics_move_cursor(1)){
                    move_reader(1);
                };
                break;

            default:
                if(input < 64){
                    if (key.caps){
                        if(key.shift){
                            if(scantableCaps[input] >= 'A' && scantableCaps[input] <= 'Z'){
                                graphics_write_char(scantable[input]);
                                append_reader(scantable[input]);
                            }
                            else{
                                graphics_write_char(scantableCaps[input]);
                                append_reader(scantableCaps[input]);
                            }
                        }
                        else{
                            if(scantableCaps[input] >= 'A' && scantableCaps[input] <= 'Z'){
                                graphics_write_char(scantableCaps[input]);
                                append_reader(scantableCaps[input]);
                            }
                            else{
                                graphics_write_char(scantable[input]);
                                append_reader(scantable[input]);
                            }
                        }
                    }
                    else {
                        if (key.shift){
                            graphics_write_char(scantableCaps[input]);
                            append_reader(scantableCaps[input]);
                        }
                        else{
                            graphics_write_char(scantable[input]);
                            append_reader(scantable[input]);
                        }
                    }
                }
                break;
        }
}