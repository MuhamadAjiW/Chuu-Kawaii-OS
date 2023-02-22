#include "keyboard.h"
#include "../lib-header/portio.h"
#include "../lib-header/framebuffer.h"
#include "../cpu/isr.h"

uint8_t caps = 0;
uint8_t shift = 0;
char buffer[4];

static void keyboard_callback(){
    uint8_t scanner = in(0x60);
    keyboardDriver(scanner);


    int_toString(scanner, buffer);
    //framebuffer_printDef(buffer);
}

void init_keyboard(){
    register_interrupt_handler(33, keyboard_callback);
}

void keyboardDriver(uint8_t input){
        switch (input){
            case 58:
                if(caps){
                    caps = 0;
                }
                else{
                    caps = 1;
                }
                break;
            case 42:
                shift = 1;
                break;
            case 170:
                shift = 0;
                break;
            case 54:
                shift = 1;
                break;
            case 182:
                shift = 0;
                break;

            case 2:
                if(shift){
                    framebuffer_printDef("!");
                } else{
                    framebuffer_printDef("1");
                }
                break;
            case 3:
                if(shift){
                    framebuffer_printDef("@");
                } else{
                    framebuffer_printDef("2");
                }
                break;
            case 4:
                if(shift){
                    framebuffer_printDef("#");
                } else{
                    framebuffer_printDef("3");
                }
                break;
            case 5:
                if(shift){
                    framebuffer_printDef("$");
                } else{
                    framebuffer_printDef("4");
                }
                break;
            case 6:
                if(shift){
                    framebuffer_printDef("%%");
                } else{
                    framebuffer_printDef("5");
                }
                break;
            case 7:
                if(shift){
                    framebuffer_printDef("^");
                } else{
                    framebuffer_printDef("6");
                }
                break;
            case 8:
                if(shift){
                    framebuffer_printDef("&");
                } else{
                    framebuffer_printDef("7");
                }
                break;
            case 9:
                if(shift){
                    framebuffer_printDef("*");
                } else{
                    framebuffer_printDef("8");
                }
                break;
            case 10:
                if(shift){
                    framebuffer_printDef("(");
                } else{
                    framebuffer_printDef("9");
                }
                break;
            case 11:
                if(shift){
                    framebuffer_printDef(")");
                } else{
                    framebuffer_printDef("0");
                }
                break;
            case 12:
                if(shift){
                    framebuffer_printDef("_");
                } else{
                    framebuffer_printDef("-");
                }
                break;
            case 13:
                if(shift){
                    framebuffer_printDef("+");
                } else{
                    framebuffer_printDef("=");
                }
                break;
            case 14:
                framebuffer_backspace();
                break;

            case 15:
                framebuffer_printDef("    ");
                break;
            case 16:
                if(caps ^ shift){
                    framebuffer_printDef("Q");
                }
                else{
                    framebuffer_printDef("q");
                }
                break;
            case 17:
                if(caps ^ shift){
                    framebuffer_printDef("W");
                }
                else{
                    framebuffer_printDef("w");
                }
                break;
            case 18:
                if(caps ^ shift){
                    framebuffer_printDef("E");
                }
                else{
                    framebuffer_printDef("e");
                }
                break;
            case 19:
                if(caps ^ shift){
                    framebuffer_printDef("R");
                }
                else{
                    framebuffer_printDef("r");
                }
                break;
            case 20:
                if(caps ^ shift){
                    framebuffer_printDef("T");
                }
                else{
                    framebuffer_printDef("t");
                }
                break;
            case 21:
                if(caps ^ shift){
                    framebuffer_printDef("Y");
                }
                else{
                    framebuffer_printDef("y");
                }
                break;
            case 22:
                if(caps ^ shift){
                    framebuffer_printDef("U");
                }
                else{
                    framebuffer_printDef("u");
                }
                break;
            case 23:
                if(caps ^ shift){
                    framebuffer_printDef("I");
                }
                else{
                    framebuffer_printDef("i");
                }
                break;
            case 24:
                if(caps ^ shift){
                    framebuffer_printDef("O");
                }
                else{
                    framebuffer_printDef("o");
                }
                break;
            case 25:
                if(caps ^ shift){
                    framebuffer_printDef("P");
                }
                else{
                    framebuffer_printDef("p");
                }
                break;
            case 26:
                if(shift){
                    framebuffer_printDef("{");
                } else{
                    framebuffer_printDef("[");
                }
                break;
            case 27:
                if(shift){
                    framebuffer_printDef("}");
                } else{
                    framebuffer_printDef("]");
                }
                break;
            case 43:
                if(shift){
                    framebuffer_printDef("|");
                } else{
                    framebuffer_printDef("\\");
                }
                break;

            case 30:
                if(caps ^ shift){
                    framebuffer_printDef("A");
                }
                else{
                    framebuffer_printDef("a");
                }
                break;
            case 31:
                if(caps ^ shift){
                    framebuffer_printDef("S");
                }
                else{
                    framebuffer_printDef("s");
                }
                break;
            case 32:
                if(caps ^ shift){
                    framebuffer_printDef("D");
                }
                else{
                    framebuffer_printDef("d");
                }
                break;
            case 33:
                if(caps ^ shift){
                    framebuffer_printDef("F");
                }
                else{
                    framebuffer_printDef("f");
                }
                break;
            case 34:
                if(caps ^ shift){
                    framebuffer_printDef("G");
                }
                else{
                    framebuffer_printDef("g");
                }
                break;
            case 35:
                if(caps ^ shift){
                    framebuffer_printDef("H");
                }
                else{
                    framebuffer_printDef("h");
                }
                break;
            case 36:
                if(caps ^ shift){
                    framebuffer_printDef("J");
                }
                else{
                    framebuffer_printDef("j");
                }
                break;
            case 37:
                if(caps ^ shift){
                    framebuffer_printDef("K");
                }
                else{
                    framebuffer_printDef("k");
                }
                break;
            case 38:
                if(caps ^ shift){
                    framebuffer_printDef("L");
                }
                else{
                    framebuffer_printDef("l");
                }
                break;
            case 39:
                if(shift){
                    framebuffer_printDef(":");
                } else{
                    framebuffer_printDef(";");
                }
                break;
            case 40:
                if(shift){
                    framebuffer_printDef("\"");
                } else{
                    framebuffer_printDef("'");
                }
                break;

            case 44:
                if(caps ^ shift){
                    framebuffer_printDef("Z");
                }
                else{
                    framebuffer_printDef("z");
                }
                break;
            case 45:
                if(caps ^ shift){
                    framebuffer_printDef("X");
                }
                else{
                    framebuffer_printDef("x");
                }
                break;
            case 46:
                if(caps ^ shift){
                    framebuffer_printDef("C");
                }
                else{
                    framebuffer_printDef("c");
                }
                break;
            case 47:
                if(caps ^ shift){
                    framebuffer_printDef("V");
                }
                else{
                    framebuffer_printDef("v");
                }
                break;
            case 48:
                if(caps ^ shift){
                    framebuffer_printDef("B");
                }
                else{
                    framebuffer_printDef("b");
                }
                break;
            case 49:
                if(caps ^ shift){
                    framebuffer_printDef("N");
                }
                else{
                    framebuffer_printDef("n");
                }
                break;
            case 50:
                if(caps ^ shift){
                    framebuffer_printDef("M");
                }
                else{
                    framebuffer_printDef("m");
                }
                break;
            case 51:
                if(shift){
                    framebuffer_printDef("<");
                } else{
                    framebuffer_printDef(",");
                }
                break;
            case 52:
                if(shift){
                    framebuffer_printDef(">");
                } else{
                    framebuffer_printDef(".");
                }
                break;
            case 53:
                if(shift){
                    framebuffer_printDef("?");
                } else{
                    framebuffer_printDef("/");
                }
                break;

            case 28:
                framebuffer_printDef("\n");
                break;
            case 57:
                framebuffer_printDef(" ");
                break;

            case 75:
                framebuffer_move_cursor(-1);
                break;
            case 77:
                framebuffer_move_cursor(1);
                break;

            default:
                break;
        }
}
