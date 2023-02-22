#include "keyboard.h"
#include "../shell.h"
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
                    append_reader('!');
                } else{
                    framebuffer_printDef("1");
                    append_reader('1');
                }
                break;
            case 3:
                if(shift){
                    framebuffer_printDef("@");
                    append_reader('@');
                } else{
                    framebuffer_printDef("2");
                    append_reader('2');
                }
                break;
            case 4:
                if(shift){
                    framebuffer_printDef("#");
                    append_reader('#');
                } else{
                    framebuffer_printDef("3");
                    append_reader('3');
                }
                break;
            case 5:
                if(shift){
                    framebuffer_printDef("$");
                    append_reader('$');
                } else{
                    framebuffer_printDef("4");
                    append_reader('4');
                }
                break;
            case 6:
                if(shift){
                    framebuffer_printDef("%%");
                    append_reader('%');
                } else{
                    framebuffer_printDef("5");
                    append_reader('5');
                }
                break;
            case 7:
                if(shift){
                    framebuffer_printDef("^");
                    append_reader('^');
                } else{
                    framebuffer_printDef("6");
                    append_reader('6');
                }
                break;
            case 8:
                if(shift){
                    framebuffer_printDef("&");
                    append_reader('&');
                } else{
                    framebuffer_printDef("7");
                    append_reader('7');
                }
                break;
            case 9:
                if(shift){
                    framebuffer_printDef("*");
                    append_reader('*');
                } else{
                    framebuffer_printDef("8");
                    append_reader('8');
                }
                break;
            case 10:
                if(shift){
                    framebuffer_printDef("(");
                    append_reader('(');
                } else{
                    framebuffer_printDef("9");
                    append_reader('9');
                }
                break;
            case 11:
                if(shift){
                    framebuffer_printDef(")");
                    append_reader(')');
                } else{
                    framebuffer_printDef("0");
                    append_reader('0');
                }
                break;
            case 12:
                if(shift){
                    framebuffer_printDef("_");
                    append_reader('_');
                } else{
                    framebuffer_printDef("-");
                    append_reader('-');
                }
                break;
            case 13:
                if(shift){
                    framebuffer_printDef("+");
                    append_reader('+');
                } else{
                    framebuffer_printDef("=");
                    append_reader('=');
                }
                break;
            case 14:
                if (framebuffer_backspace()){
                    backspace_reader();
                }
                break;

            case 15:
                framebuffer_printDef("    ");
                append_reader(' ');
                append_reader(' ');
                append_reader(' ');
                append_reader(' ');
                break;
            case 16:
                if(caps ^ shift){
                    framebuffer_printDef("Q");
                    append_reader('Q');
                }
                else{
                    framebuffer_printDef("q");
                    append_reader('q');
                }
                break;
            case 17:
                if(caps ^ shift){
                    framebuffer_printDef("W");
                    append_reader('W');
                }
                else{
                    framebuffer_printDef("w");
                    append_reader('w');
                }
                break;
            case 18:
                if(caps ^ shift){
                    framebuffer_printDef("E");
                    append_reader('E');
                }
                else{
                    framebuffer_printDef("e");
                    append_reader('e');
                }
                break;
            case 19:
                if(caps ^ shift){
                    framebuffer_printDef("R");
                    append_reader('R');
                }
                else{
                    framebuffer_printDef("r");
                    append_reader('r');
                }
                break;
            case 20:
                if(caps ^ shift){
                    framebuffer_printDef("T");
                    append_reader('T');
                }
                else{
                    framebuffer_printDef("t");
                    append_reader('t');
                }
                break;
            case 21:
                if(caps ^ shift){
                    framebuffer_printDef("Y");
                    append_reader('Y');
                }
                else{
                    framebuffer_printDef("y");
                    append_reader('y');
                }
                break;
            case 22:
                if(caps ^ shift){
                    framebuffer_printDef("U");
                    append_reader('U');
                }
                else{
                    framebuffer_printDef("u");
                    append_reader('u');
                }
                break;
            case 23:
                if(caps ^ shift){
                    framebuffer_printDef("I");
                    append_reader('I');
                }
                else{
                    framebuffer_printDef("i");
                    append_reader('i');
                }
                break;
            case 24:
                if(caps ^ shift){
                    framebuffer_printDef("O");
                    append_reader('O');
                }
                else{
                    framebuffer_printDef("o");
                    append_reader('o');
                }
                break;
            case 25:
                if(caps ^ shift){
                    framebuffer_printDef("P");
                    append_reader('P');
                }
                else{
                    framebuffer_printDef("p");
                    append_reader('p');
                }
                break;
            case 26:
                if(shift){
                    framebuffer_printDef("{");
                    append_reader('{');
                } else{
                    framebuffer_printDef("[");
                    append_reader('[');
                }
                break;
            case 27:
                if(shift){
                    framebuffer_printDef("}");
                    append_reader('}');
                } else{
                    framebuffer_printDef("]");
                    append_reader(']');
                }
                break;
            case 43:
                if(shift){
                    framebuffer_printDef("|");
                    append_reader('|');
                } else{
                    framebuffer_printDef("\\");
                    append_reader('\\');
                }
                break;

            case 30:
                if(caps ^ shift){
                    framebuffer_printDef("A");
                    append_reader('A');
                }
                else{
                    framebuffer_printDef("a");
                    append_reader('a');
                }
                break;
            case 31:
                if(caps ^ shift){
                    framebuffer_printDef("S");
                    append_reader('S');
                }
                else{
                    framebuffer_printDef("s");
                    append_reader('s');
                }
                break;
            case 32:
                if(caps ^ shift){
                    framebuffer_printDef("D");
                    append_reader('D');
                }
                else{
                    framebuffer_printDef("d");
                    append_reader('d');
                }
                break;
            case 33:
                if(caps ^ shift){
                    framebuffer_printDef("F");
                    append_reader('F');
                }
                else{
                    framebuffer_printDef("f");
                    append_reader('f');
                }
                break;
            case 34:
                if(caps ^ shift){
                    framebuffer_printDef("G");
                    append_reader('G');
                }
                else{
                    framebuffer_printDef("g");
                    append_reader('g');
                }
                break;
            case 35:
                if(caps ^ shift){
                    framebuffer_printDef("H");
                    append_reader('H');
                }
                else{
                    framebuffer_printDef("h");
                    append_reader('h');
                }
                break;
            case 36:
                if(caps ^ shift){
                    framebuffer_printDef("J");
                    append_reader('J');
                }
                else{
                    framebuffer_printDef("j");
                    append_reader('j');
                }
                break;
            case 37:
                if(caps ^ shift){
                    framebuffer_printDef("K");
                    append_reader('K');
                }
                else{
                    framebuffer_printDef("k");
                    append_reader('k');
                }
                break;
            case 38:
                if(caps ^ shift){
                    framebuffer_printDef("L");
                    append_reader('L');
                }
                else{
                    framebuffer_printDef("l");
                    append_reader('l');
                }
                break;
            case 39:
                if(shift){
                    framebuffer_printDef(":");
                    append_reader(':');
                } else{
                    framebuffer_printDef(";");
                    append_reader(';');
                }
                break;
            case 40:
                if(shift){
                    framebuffer_printDef("\"");
                    append_reader('"');
                } else{
                    framebuffer_printDef("'");
                    append_reader('\'');
                }
                break;

            case 44:
                if(caps ^ shift){
                    framebuffer_printDef("Z");
                    append_reader('Z');
                }
                else{
                    framebuffer_printDef("z");
                    append_reader('z');
                }
                break;
            case 45:
                if(caps ^ shift){
                    framebuffer_printDef("X");
                    append_reader('X');
                }
                else{
                    framebuffer_printDef("x");
                    append_reader('x');
                }
                break;
            case 46:
                if(caps ^ shift){
                    framebuffer_printDef("C");
                    append_reader('C');
                }
                else{
                    framebuffer_printDef("c");
                    append_reader('c');
                }
                break;
            case 47:
                if(caps ^ shift){
                    framebuffer_printDef("V");
                    append_reader('V');
                }
                else{
                    framebuffer_printDef("v");
                    append_reader('v');
                }
                break;
            case 48:
                if(caps ^ shift){
                    framebuffer_printDef("B");
                    append_reader('B');
                }
                else{
                    framebuffer_printDef("b");
                    append_reader('b');
                }
                break;
            case 49:
                if(caps ^ shift){
                    framebuffer_printDef("N");
                    append_reader('N');
                }
                else{
                    framebuffer_printDef("n");
                    append_reader('n');
                }
                break;
            case 50:
                if(caps ^ shift){
                    framebuffer_printDef("M");
                    append_reader('M');
                }
                else{
                    framebuffer_printDef("m");
                    append_reader('m');
                }
                break;
            case 51:
                if(shift){
                    framebuffer_printDef("<");
                    append_reader('<');
                } else{
                    framebuffer_printDef(",");
                    append_reader(',');
                }
                break;
            case 52:
                if(shift){
                    framebuffer_printDef(">");
                    append_reader('>');
                } else{
                    framebuffer_printDef(".");
                    append_reader('.');
                }
                break;
            case 53:
                if(shift){
                    framebuffer_printDef("?");
                    append_reader('?');
                } else{
                    framebuffer_printDef("/");
                    append_reader('/');
                }
                break;

            case 28:
                append_reader('\n');
                newline_shell();
                break;
            case 57:
                framebuffer_printDef(" ");
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
