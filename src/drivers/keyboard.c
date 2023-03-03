#include "../lib-header/keyboard.h"
#include "../lib-header/shell.h"
#include "../lib-header/portio.h"
#include "../lib-header/framebuffer.h"
#include "../lib-header/isr.h"
#include "../lib-header/idt.h"
#include "../lib-header/string.h"

extern void* isr_stub_table[];

static keyboardDriverState key ={
    .currentIdx = 0,
    .maxIdx = 0,
    .shift = 0,
    .caps = 0,
    .keyboard_input_on = 0
};

char buffer[4];

static void keyboard_callback(){
    uint8_t scanner = in(0x60);
    if(key.keyboard_input_on){
        keyboardDriver(scanner);
    }

    int_toString(scanner, buffer);
    //framebuffer_printDef(buffer);
}

void activate_keyboard_interrupt(){
    set_idt_gate(33, isr_stub_table[33]);
    register_interrupt_handler(33, keyboard_callback);
}

void keyboard_state_activate(){
    key.keyboard_input_on = 1;
}

void keyboard_state_deactivate(){
    key.keyboard_input_on = 0;
}

void clear_reader(){
    key.shellReader[0] = 0;
    key.currentIdx = 0;
    key.maxIdx = 0;
}

void append_reader(char in){
    if(key.maxIdx < 500){
        if(!(in == '\n')){
            for(int i = key.maxIdx; i > key.currentIdx; i--){
                key.shellReader[i] = key.shellReader[i-1];
            }

            key.shellReader[key.currentIdx] = in;
            key.maxIdx++;
            key.currentIdx++;
        }
        else{
            key.shellReader[key.currentIdx] = 0;
            execute_reader();
            clear_reader();
        }
    }
    else{
        framebuffer_printDef("\nBuffer overflowed, resetting buffer...\n");
        clear_reader();
    }
}

void move_reader(int direction){
    key.currentIdx = key.currentIdx + direction;
}

void backspace_reader(){
    for(int i = key.currentIdx-1; i < key.maxIdx-1; i++){
        key.shellReader[i] = key.shellReader[i+1];
    }
    key.currentIdx--;
    key.maxIdx--;
}

char* get_keyboard_buffer(){
    return key.shellReader;
}

void keyboardDriver(uint8_t input){
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

            case 2:
                if(key.shift){
                    framebuffer_insert_char('!');
                    append_reader('!');
                } else{
                    framebuffer_insert_char('1');
                    append_reader('1');
                }
                break;
            case 3:
                if(key.shift){
                    framebuffer_insert_char('@');
                    append_reader('@');
                } else{
                    framebuffer_insert_char('2');
                    append_reader('2');
                }
                break;
            case 4:
                if(key.shift){
                    framebuffer_insert_char('#');
                    append_reader('#');
                } else{
                    framebuffer_insert_char('3');
                    append_reader('3');
                }
                break;
            case 5:
                if(key.shift){
                    framebuffer_insert_char('$');
                    append_reader('$');
                } else{
                    framebuffer_insert_char('4');
                    append_reader('4');
                }
                break;
            case 6:
                if(key.shift){
                    framebuffer_insert_char('%');
                    append_reader('%');
                } else{
                    framebuffer_insert_char('5');
                    append_reader('5');
                }
                break;
            case 7:
                if(key.shift){
                    framebuffer_insert_char('^');
                    append_reader('^');
                } else{
                    framebuffer_insert_char('6');
                    append_reader('6');
                }
                break;
            case 8:
                if(key.shift){
                    framebuffer_insert_char('&');
                    append_reader('&');
                } else{
                    framebuffer_insert_char('7');
                    append_reader('7');
                }
                break;
            case 9:
                if(key.shift){
                    framebuffer_insert_char('*');
                    append_reader('*');
                } else{
                    framebuffer_insert_char('8');
                    append_reader('8');
                }
                break;
            case 10:
                if(key.shift){
                    framebuffer_insert_char('(');
                    append_reader('(');
                } else{
                    framebuffer_insert_char('9');
                    append_reader('9');
                }
                break;
            case 11:
                if(key.shift){
                    framebuffer_insert_char(')');
                    append_reader(')');
                } else{
                    framebuffer_insert_char('0');
                    append_reader('0');
                }
                break;
            case 12:
                if(key.shift){
                    framebuffer_insert_char('_');
                    append_reader('_');
                } else{
                    framebuffer_insert_char('-');
                    append_reader('-');
                }
                break;
            case 13:
                if(key.shift){
                    framebuffer_insert_char('+');
                    append_reader('+');
                } else{
                    framebuffer_insert_char('=');
                    append_reader('=');
                }
                break;
            case 14:
                if (framebuffer_backspace()){
                    backspace_reader();
                }
                break;

            case 15:
                framebuffer_insert_char(' ');
                framebuffer_insert_char(' ');
                framebuffer_insert_char(' ');
                framebuffer_insert_char(' ');
                append_reader(' ');
                append_reader(' ');
                append_reader(' ');
                append_reader(' ');
                break;
            case 16:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('Q');
                    append_reader('Q');
                }
                else{
                    framebuffer_insert_char('q');
                    append_reader('q');
                }
                break;
            case 17:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('W');
                    append_reader('W');
                }
                else{
                    framebuffer_insert_char('w');
                    append_reader('w');
                }
                break;
            case 18:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('E');
                    append_reader('E');
                }
                else{
                    framebuffer_insert_char('e');
                    append_reader('e');
                }
                break;
            case 19:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('R');
                    append_reader('R');
                }
                else{
                    framebuffer_insert_char('r');
                    append_reader('r');
                }
                break;
            case 20:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('T');
                    append_reader('T');
                }
                else{
                    framebuffer_insert_char('t');
                    append_reader('t');
                }
                break;
            case 21:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('Y');
                    append_reader('Y');
                }
                else{
                    framebuffer_insert_char('y');
                    append_reader('y');
                }
                break;
            case 22:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('U');
                    append_reader('U');
                }
                else{
                    framebuffer_insert_char('u');
                    append_reader('u');
                }
                break;
            case 23:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('I');
                    append_reader('I');
                }
                else{
                    framebuffer_insert_char('i');
                    append_reader('i');
                }
                break;
            case 24:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('O');
                    append_reader('O');
                }
                else{
                    framebuffer_insert_char('o');
                    append_reader('o');
                }
                break;
            case 25:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('P');
                    append_reader('P');
                }
                else{
                    framebuffer_insert_char('p');
                    append_reader('p');
                }
                break;
            case 26:
                if(key.shift){
                    framebuffer_insert_char('{');
                    append_reader('{');
                } else{
                    framebuffer_insert_char('[');
                    append_reader('[');
                }
                break;
            case 27:
                if(key.shift){
                    framebuffer_insert_char('}');
                    append_reader('}');
                } else{
                    framebuffer_insert_char(']');
                    append_reader(']');
                }
                break;
            case 43:
                if(key.shift){
                    framebuffer_insert_char('|');
                    append_reader('|');
                } else{
                    framebuffer_insert_char('\\');
                    append_reader('\\');
                }
                break;

            case 30:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('A');
                    append_reader('A');
                }
                else{
                    framebuffer_insert_char('a');
                    append_reader('a');
                }
                break;
            case 31:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('S');
                    append_reader('S');
                }
                else{
                    framebuffer_insert_char('s');
                    append_reader('s');
                }
                break;
            case 32:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('D');
                    append_reader('D');
                }
                else{
                    framebuffer_insert_char('d');
                    append_reader('d');
                }
                break;
            case 33:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('F');
                    append_reader('F');
                }
                else{
                    framebuffer_insert_char('f');
                    append_reader('f');
                }
                break;
            case 34:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('G');
                    append_reader('G');
                }
                else{
                    framebuffer_insert_char('g');
                    append_reader('g');
                }
                break;
            case 35:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('H');
                    append_reader('H');
                }
                else{
                    framebuffer_insert_char('h');
                    append_reader('h');
                }
                break;
            case 36:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('J');
                    append_reader('J');
                }
                else{
                    framebuffer_insert_char('j');
                    append_reader('j');
                }
                break;
            case 37:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('K');
                    append_reader('K');
                }
                else{
                    framebuffer_insert_char('k');
                    append_reader('k');
                }
                break;
            case 38:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('L');
                    append_reader('L');
                }
                else{
                    framebuffer_insert_char('l');
                    append_reader('l');
                }
                break;
            case 39:
                if(key.shift){
                    framebuffer_insert_char(':');
                    append_reader(':');
                } else{
                    framebuffer_insert_char(';');
                    append_reader(';');
                }
                break;
            case 40:
                if(key.shift){
                    framebuffer_insert_char('"');
                    append_reader('"');
                } else{
                    framebuffer_insert_char('\'');
                    append_reader('\'');
                }
                break;

            case 44:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('Z');
                    append_reader('Z');
                }
                else{
                    framebuffer_insert_char('z');
                    append_reader('z');
                }
                break;
            case 45:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('X');
                    append_reader('X');
                }
                else{
                    framebuffer_insert_char('x');
                    append_reader('x');
                }
                break;
            case 46:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('C');
                    append_reader('C');
                }
                else{
                    framebuffer_insert_char('c');
                    append_reader('c');
                }
                break;
            case 47:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('V');
                    append_reader('V');
                }
                else{
                    framebuffer_insert_char('v');
                    append_reader('v');
                }
                break;
            case 48:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('B');
                    append_reader('B');
                }
                else{
                    framebuffer_insert_char('b');
                    append_reader('b');
                }
                break;
            case 49:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('N');
                    append_reader('N');
                }
                else{
                    framebuffer_insert_char('n');
                    append_reader('n');
                }
                break;
            case 50:
                if(key.caps ^ key.shift){
                    framebuffer_insert_char('M');
                    append_reader('M');
                }
                else{
                    framebuffer_insert_char('m');
                    append_reader('m');
                }
                break;
            case 51:
                if(key.shift){
                    framebuffer_insert_char('<');
                    append_reader('<');
                } else{
                    framebuffer_insert_char(',');
                    append_reader(',');
                }
                break;
            case 52:
                if(key.shift){
                    framebuffer_insert_char('>');
                    append_reader('>');
                } else{
                    framebuffer_insert_char('.');
                    append_reader('.');
                }
                break;
            case 53:
                if(key.shift){
                    framebuffer_insert_char('?');
                    append_reader('?');
                } else{
                    framebuffer_insert_char('/');
                    append_reader('/');
                }
                break;

            case 28:
                append_reader('\n');
                newline_shell();
                break;
            case 57:
                framebuffer_insert_char(' ');
                append_reader(' ');
                break;

            case 75:
                if (framebuffer_move_cursor(-1)){
                    move_reader(-1);
                };
                break;
            case 77:
                if (framebuffer_move_cursor(1)){
                    move_reader(1);
                };
                break;

            default:
                break;
        }
}
