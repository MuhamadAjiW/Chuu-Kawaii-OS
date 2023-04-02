#include "../lib-header/keyboard.h"
#include "../lib-header/memory_manager.h"
#include "../lib-header/shell.h"
#include "../lib-header/portio.h"
#include "../lib-header/isr.h"
#include "../lib-header/idt.h"
#include "../lib-header/string.h"
#include "../lib-header/graphics.h"

static const char scantable[64] = {
    0,0,'1','2','3','4','5','6','7','8',
    '9','0','-','=',0,0,'q','w','e','r',
    't','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';',
    '\'',0,0,0,'z','x','c','v','b','n',
    'm',',','.','/',0,0,0,' ',0,0,
    0,0,0,0
};
static const char scantableCaps[64] = {
    0,0,'!','@','#','$','%','^','&','*',
    '(',')','_','+',0,0,'Q','W','E','R',
    'T','Y','U','I','O','P','{','}','\n',0,
    'A','S','D','F','G','H','J','K','L',':',
    '"',0,0,0,'Z','X','C','V','B','N',
    'M','<','>','?',0,0,0,' ',0,0,
    0,0,0,0
};

static keyboardDriverState key ={
    .last = 0,
    //.keyboard_buffer = 0,
    //.buffersize = KEYBOARD_BUFFER_SIZE,
    //.currentIdx = 0,
    //.maxIdx = 0,
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
    /*
    if(key.keyboard_buffer == 0){
        key.keyboard_buffer = (char*) malloc (sizeof(char) * KEYBOARD_BUFFER_SIZE);
    }
    */
    key.keyboard_input_on = 1;
}

void keyboard_state_deactivate(){
    key.keyboard_input_on = 0;
}
/*
void clear_reader(){
    key.buffersize = KEYBOARD_BUFFER_SIZE;
    free(key.keyboard_buffer);
    key.keyboard_buffer = (char*) malloc (sizeof(char) * KEYBOARD_BUFFER_SIZE);;
    key.keyboard_buffer[0] = 0;
    key.currentIdx = 0;
    key.maxIdx = 0;
}
void append_reader(char in){
    key.last[0] = in;
    if(key.maxIdx < key.buffersize - 1){
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
        key.buffersize += KEYBOARD_BUFFER_SIZE;
        key.keyboard_buffer = (char*) realloc (key.keyboard_buffer, sizeof(char) * key.buffersize);
        append_reader(in);
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
*/
char get_keyboard_last_key(){
    char temp = key.last;
    key.last = NULL_CHAR;
    return temp;
}

void set_keyboard_last_key(char in){
    key.last = in;
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
                set_keyboard_last_key(BACKSPACE_CHAR);
                /*
                if (graphics_backspace()){
                    backspace_reader();
                }
                */
                break;

            case 15:
                set_keyboard_last_key(TAB_CHAR);
                //graphics_write_char(' ');
                //graphics_write_char(' ');
                //graphics_write_char(' ');
                //graphics_write_char(' ');
                //append_reader(' ');
                //append_reader(' ');
                //append_reader(' ');
                //append_reader(' ');
                break;

            case 75:
                set_keyboard_last_key(LARROW_CHAR);
                /*
                if (graphics_move_cursor(-1)){
                    //move_reader(-1);
                };
                */
                break;
            case 77:
                set_keyboard_last_key(RARROW_CHAR);
                /*
                if (graphics_move_cursor(1)){
                    move_reader(1);
                };
                */
                break;

            default:
                if(input < 64){
                    if(scantable[input] != 0 && scantableCaps[input] != 0){
                        if (key.caps){
                            if(key.shift){
                                if(scantableCaps[input] >= 'A' && scantableCaps[input] <= 'Z'){
                                    //graphics_write_char(scantable[input]);
                                    //append_reader(scantable[input]);
                                    set_keyboard_last_key(scantable[input]);
                                }
                                else{
                                    //graphics_write_char(scantableCaps[input]);
                                    //append_reader(scantableCaps[input]);
                                    set_keyboard_last_key(scantableCaps[input]);
                                }
                            }
                            else{
                                if(scantableCaps[input] >= 'A' && scantableCaps[input] <= 'Z'){
                                    //graphics_write_char(scantableCaps[input]);
                                    //append_reader(scantableCaps[input]);
                                    set_keyboard_last_key(scantableCaps[input]);
                                }
                                else{
                                    //graphics_write_char(scantable[input]);
                                    //append_reader(scantable[input]);
                                    set_keyboard_last_key(scantable[input]);
                                }
                            }
                        }
                        else {
                            if (key.shift){
                                //graphics_write_char(scantableCaps[input]);
                                //append_reader(scantableCaps[input]);
                                set_keyboard_last_key(scantableCaps[input]);
                            }
                            else{
                                //graphics_write_char(scantable[input]);
                                //append_reader(scantable[input]);
                                set_keyboard_last_key(scantable[input]);
                            }
                        }
                    }
                }
                break;
        }
}