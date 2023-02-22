#include "lib-header/framebuffer.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/portio.h"

void framebuffer_set_cursor(uint8_t r, uint8_t c) {
    uint16_t position;
    
    position = r * 80 + c;
    out(CURSOR_PORT_CMD, 0x0f);
    out(CURSOR_PORT_DATA, (uint8_t) position);
    out(CURSOR_PORT_CMD, 0x0e);
    out(CURSOR_PORT_DATA, (uint8_t) (position >> 8));
}

uint16_t framebuffer_get_cursor(){
    uint16_t position;

    position = 0;
    out(CURSOR_PORT_CMD, 0x0f);
    position |= in(CURSOR_PORT_DATA);
    out(CURSOR_PORT_CMD, 0x0e);
    position |= (in(CURSOR_PORT_DATA) << 8);

    return position;
}

int framebuffer_find_edge(int row){
    uint8_t col = 0;
    uint8_t* location;
    location = (uint8_t*) MEMORY_FRAMEBUFFER + (row* 80 + col)*2;
    while (*location != 0 && col < 79){
        col++;
        location = (uint8_t*) MEMORY_FRAMEBUFFER + (row* 80 + col)*2;
    }
    return col;
}

void framebuffer_move_cursor(int direction){
    uint16_t cursor = framebuffer_get_cursor();
    uint8_t row = cursor/80;
    uint8_t col = cursor%80;
    switch (direction){
        case (1):
            if((col == 79 || *(MEMORY_FRAMEBUFFER+(row* 80 + col)*2) == 0) && row != 24){
                framebuffer_set_cursor(row+1, 0);
            }
            else{
                if(*(MEMORY_FRAMEBUFFER+(row* 80 + col)*2) != 0){
                    framebuffer_set_cursor(row,col+1);
                }
            }
            break;
        case (-1):
            if(col != 0){
                framebuffer_set_cursor(row,col-1);
            }
            else{
                if(row != 0){
                    framebuffer_set_cursor(row-1, framebuffer_find_edge(row-1));
                }
            }
            break;
        
        default:
            break;
    }
}

void framebuffer_write(uint8_t row, uint8_t col, char c, uint8_t fg, uint8_t bg) {
    uint8_t * location;
    uint8_t color;

    location = (uint8_t*) MEMORY_FRAMEBUFFER + (row* 80 + col)*2;
    color = (bg << 4) | (fg & 0x0f);

    memset(location, c, 1);
    memset(location+1, color, 1);
}

void framebuffer_clear(void) {
    for (int row = 0; row < 25; row++){
        for (int col = 0; col < 80; col++){
            framebuffer_write(row, col, 0, 0x0, 0xf);
        }
    }
    framebuffer_set_cursor(0, 0);
}

void framebuffer_scroll(){

    memcpy(MEMORY_FRAMEBUFFER, MEMORY_FRAMEBUFFER + 2*80, 2*25*80);

    for(int col2 = 0; col2 < 80; col2++){
        framebuffer_write(24, col2, 0, 0x0, 0xf);
    }
}


void framebuffer_print(char* string, uint8_t fg, uint8_t bg){
    int i = 0;
    uint16_t cursor = framebuffer_get_cursor();
    uint8_t row = cursor/80;
    uint8_t col = cursor%80;
    while (string[i] != 0){

        if(string[i] != '\n'){
            framebuffer_write(row, col, string[i], fg, bg);

            col += 1;
            if(col > 79){
                if(row == 24){
                    framebuffer_scroll();
                }
                else{
                    row += 1;
                }
                col = 0;
            }
        }
        else{
            if(row == 24){
                framebuffer_scroll();
            }
            else{
                row += 1;
            }
            col = 0;
        }
        

        i++;
    }
    framebuffer_set_cursor(row, col);
}

void framebuffer_printDef(char* string){
    int i = 0;
    uint16_t cursor = framebuffer_get_cursor();
    uint8_t row = cursor/80;
    uint8_t col = cursor%80;
    while (string[i] != 0){

        if(string[i] != '\n'){
            framebuffer_write(row, col, string[i], 0, 0xf);

            col += 1;
            if(col > 79){
                if(row == 24){
                    framebuffer_scroll();
                }
                else{
                    row += 1;
                }
                col = 0;
            }
        }
        else{
            if(row == 24){
                framebuffer_scroll();
            }
            else{
                row += 1;
            }
            col = 0;
        }
        

        i++;
    }
    framebuffer_set_cursor(row, col);
}

void framebuffer_backspace(){
    uint16_t cursor = framebuffer_get_cursor();
    uint8_t row = cursor/80;
    uint8_t col = cursor%80;

    if(col != 0 && col != 79){
         memcpy(MEMORY_FRAMEBUFFER + (row* 80 + col-1)*2, MEMORY_FRAMEBUFFER + (row* 80 + col)*2, (MEMORY_FRAMEBUFFER + (row+1)*80*2) - (MEMORY_FRAMEBUFFER + (row* 80 + col)*2));
    }
    else if (col == 79){
        framebuffer_write(row, col, 0, 0x0, 0xf);
    }
    
    framebuffer_move_cursor(-1);
}

void int_toString(int x, char str[]){
    int i = 0;
    int negative = 0;

    if(x < 0){
        x = x*(-1);
        negative = 1;
    }

    do{
        str[i] = x % 10 + '0';
        i++; 
    } while  ( (x /= 10) > 0);

    if(negative){
        str[i] = '-';
        str[i+1] = 0;
    }
    else{
        str[i] = 0;
    }

    int j, k, temp;
    for(j = 0, k = strlen(str) - 1; j < k; j++, k--){
        temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

int strlen(char str[]){
    int counter = 0;
    while(str[counter] != 0){
        counter++;
    }
    return counter;
}