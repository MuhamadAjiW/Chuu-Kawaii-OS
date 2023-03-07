#include "../lib-header/graphics.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/string.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/portio.h"
#include "../lib-header/timer.h"
#include "../lib-header/isr.h"
#include "lib-header/stdfont.h"

void initialize_vga(){
    //source
    //https://wiki.osdev.org/VGA_Hardware#Memory_Layout_in_256-color_graphics_modes
    //http://www.osdever.net/FreeVGA/home.htm

    out(0x3c2, 0x63);
    out2(0x3d4, (uint16_t) ( (0x0e << 8) + 0x11));

    out2(0x3d4, (uint16_t) ( (0x5f << 8) + 0x0));
    out2(0x3d4, (uint16_t) ( (0x4f << 8) + 0x1));
    out2(0x3d4, (uint16_t) ( (0x50 << 8) + 0x2));
    out2(0x3d4, (uint16_t) ( (0x82 << 8) + 0x3));
    out2(0x3d4, (uint16_t) ( (0x54 << 8) + 0x4));
    out2(0x3d4, (uint16_t) ( (0x80 << 8) + 0x5));
    out2(0x3d4, (uint16_t) ( (0xbf << 8) + 0x6));
    out2(0x3d4, (uint16_t) ( (0x1f << 8) + 0x7));
    out2(0x3d4, (uint16_t) ( (0x00 << 8) + 0x8));
    out2(0x3d4, (uint16_t) ( (0x41 << 8) + 0x9));
    out2(0x3d4, (uint16_t) ( (0x9c << 8) + 0x10));
    out2(0x3d4, (uint16_t) ( (0x8e << 8) + 0x11));
    out2(0x3d4, (uint16_t) ( (0x8f << 8) + 0x12));
    out2(0x3d4, (uint16_t) ( (0x28 << 8) + 0x13));
    out2(0x3d4, (uint16_t) ( (0x40 << 8) + 0x14));
    out2(0x3d4, (uint16_t) ( (0x96 << 8) + 0x15));
    out2(0x3d4, (uint16_t) ( (0xb9 << 8) + 0x16));
    out2(0x3d4, (uint16_t) ( (0xa3 << 8) + 0x17));

    out2(0x3c4, (uint16_t) ( (0x01 << 8) + 0x01));
    out2(0x3c4, (uint16_t) ( (0x0f << 8) + 0x02));
    out2(0x3c4, (uint16_t) ( (0x00 << 8) + 0x03));
    out2(0x3c4, (uint16_t) ( (0x0e << 8) + 0x04));

    out2(0x3ce, (uint16_t) ( (0x40 << 8) + 0x05));
    out2(0x3ce, (uint16_t) ( (0x05 << 8) + 0x06));

    in(0x3da);
    out(0x3c0,0x10);
    out2(0x3c0,0x41);

    in(0x3da);
    out(0x3c0,0x11);
    out2(0x3c0,0x00);

    in(0x3da);
    out(0x3c0,0x12);
    out2(0x3c0,0x0f);
    
    in(0x3da);
    out(0x3c0,0x13);
    out2(0x3c0,0x00);

    in(0x3da);
    out(0x3c0,0x14);
    out2(0x3c0,0x00);

    //palette
    //array source: https://github.com/canidlogic/vgapal
    uint8_t paletteArray[768] = {
        0,  0,  0,   0,  0, 42,   0, 42,  0,   0, 42, 42,
        42,  0,  0,  42,  0, 42,  42, 21,  0,  42, 42, 42,
        21, 21, 21,  21, 21, 63,  21, 63, 21,  21, 63, 63,
        63, 21, 21,  63, 21, 63,  63, 63, 21,  63, 63, 63,
        0,  0,  0,   5,  5,  5,   8,  8,  8,  11, 11, 11,
        14, 14, 14,  17, 17, 17,  20, 20, 20,  24, 24, 24,
        28, 28, 28,  32, 32, 32,  36, 36, 36,  40, 40, 40,
        45, 45, 45,  50, 50, 50,  56, 56, 56,  63, 63, 63,
        0,  0, 63,  16,  0, 63,  31,  0, 63,  47,  0, 63,
        63,  0, 63,  63,  0, 47,  63,  0, 31,  63,  0, 16,
        63,  0,  0,  63, 16,  0,  63, 31,  0,  63, 47,  0,
        63, 63,  0,  47, 63,  0,  31, 63,  0,  16, 63,  0,
        0, 63,  0,   0, 63, 16,   0, 63, 31,   0, 63, 47,
        0, 63, 63,   0, 47, 63,   0, 31, 63,   0, 16, 63,
        31, 31, 63,  39, 31, 63,  47, 31, 63,  55, 31, 63,
        63, 31, 63,  63, 31, 55,  63, 31, 47,  63, 31, 39,
        63, 31, 31,  63, 39, 31,  63, 47, 31,  63, 55, 31,
        63, 63, 31,  55, 63, 31,  47, 63, 31,  39, 63, 31,
        31, 63, 31,  31, 63, 39,  31, 63, 47,  31, 63, 55,
        31, 63, 63,  31, 55, 63,  31, 47, 63,  31, 39, 63,
        45, 45, 63,  49, 45, 63,  54, 45, 63,  58, 45, 63,
        63, 45, 63,  63, 45, 58,  63, 45, 54,  63, 45, 49,
        63, 45, 45,  63, 49, 45,  63, 54, 45,  63, 58, 45,
        63, 63, 45,  58, 63, 45,  54, 63, 45,  49, 63, 45,
        45, 63, 45,  45, 63, 49,  45, 63, 54,  45, 63, 58,
        45, 63, 63,  45, 58, 63,  45, 54, 63,  45, 49, 63,
        0,  0, 28,   7,  0, 28,  14,  0, 28,  21,  0, 28,
        28,  0, 28,  28,  0, 21,  28,  0, 14,  28,  0,  7,
        28,  0,  0,  28,  7,  0,  28, 14,  0,  28, 21,  0,
        28, 28,  0,  21, 28,  0,  14, 28,  0,   7, 28,  0,
        0, 28,  0,   0, 28,  7,   0, 28, 14,   0, 28, 21,
        0, 28, 28,   0, 21, 28,   0, 14, 28,   0,  7, 28,
        14, 14, 28,  17, 14, 28,  21, 14, 28,  24, 14, 28,
        28, 14, 28,  28, 14, 24,  28, 14, 21,  28, 14, 17,
        28, 14, 14,  28, 17, 14,  28, 21, 14,  28, 24, 14,
        28, 28, 14,  24, 28, 14,  21, 28, 14,  17, 28, 14,
        14, 28, 14,  14, 28, 17,  14, 28, 21,  14, 28, 24,
        14, 28, 28,  14, 24, 28,  14, 21, 28,  14, 17, 28,
        20, 20, 28,  22, 20, 28,  24, 20, 28,  26, 20, 28,
        28, 20, 28,  28, 20, 26,  28, 20, 24,  28, 20, 22,
        28, 20, 20,  28, 22, 20,  28, 24, 20,  28, 26, 20,
        28, 28, 20,  26, 28, 20,  24, 28, 20,  22, 28, 20,
        20, 28, 20,  20, 28, 22,  20, 28, 24,  20, 28, 26,
        20, 28, 28,  20, 26, 28,  20, 24, 28,  20, 22, 28,
        0,  0, 16,   4,  0, 16,   8,  0, 16,  12,  0, 16,
        16,  0, 16,  16,  0, 12,  16,  0,  8,  16,  0,  4,
        16,  0,  0,  16,  4,  0,  16,  8,  0,  16, 12,  0,
        16, 16,  0,  12, 16,  0,   8, 16,  0,   4, 16,  0,
        0, 16,  0,   0, 16,  4,   0, 16,  8,   0, 16, 12,
        0, 16, 16,   0, 12, 16,   0,  8, 16,   0,  4, 16,
        8,  8, 16,  10,  8, 16,  12,  8, 16,  14,  8, 16,
        16,  8, 16,  16,  8, 14,  16,  8, 12,  16,  8, 10,
        16,  8,  8,  16, 10,  8,  16, 12,  8,  16, 14,  8,
        16, 16,  8,  14, 16,  8,  12, 16,  8,  10, 16,  8,
        8, 16,  8,   8, 16, 10,   8, 16, 12,   8, 16, 14,
        8, 16, 16,   8, 14, 16,   8, 12, 16,   8, 10, 16,
        11, 11, 16,  12, 11, 16,  13, 11, 16,  15, 11, 16,
        16, 11, 16,  16, 11, 15,  16, 11, 13,  16, 11, 12,
        16, 11, 11,  16, 12, 11,  16, 13, 11,  16, 15, 11,
        16, 16, 11,  15, 16, 11,  13, 16, 11,  12, 16, 11,
        11, 16, 11,  11, 16, 12,  11, 16, 13,  11, 16, 15,
        11, 16, 16,  11, 15, 16,  11, 13, 16,  11, 12, 16,
        0,  0,  0,   0,  0,  0,   0,  0,  0,   0,  0,  0,
        0,  0,  0,   0,  0,  0,   0,  0,  0,   0,  0,  0
    };

    out(0x03c8, 0);
    for(int i = 0; i < 768; i += 3){
        out(0x03c9, (uint8_t) paletteArray[i]);
        out(0x03c9, (uint8_t) paletteArray[i + 1]);
        out(0x03c9, (uint8_t) paletteArray[i + 2]);
    }

    out(0x3c0, 0x20);
    return;
}

uint8_t colLimit = SCREEN_WIDTH/BLOCK_WIDTH;
uint8_t rowLimit = SCREEN_HEIGHT/BLOCK_HEIGHT;
char screenMap[SCREEN_HEIGHT/BLOCK_HEIGHT][SCREEN_WIDTH/BLOCK_WIDTH] = {0};


void graphics_clear(){
    memset(MEMORY_GRAPHICS, DEFAULT_COLOR_BG, 0x10000);
    return;
}

void graphics_clear_buffer(){
    for(int i = 0; i < rowLimit; i++){
        for(int j = 0; j < colLimit; j++){
            screenMap[i][j] = 0;
        }
    }
    memset(MEMORY_GRAPHICS, DEFAULT_COLOR_BG, 0x10000);
    graphics_set_cursor(0, 0);
    return;
}

void graphics_draw(uint16_t x, uint16_t y, uint8_t color){
    memset(MEMORY_GRAPHICS + (320 * y) + x, color, 1);
    return;
}

void graphics_draw_block(uint8_t x, uint8_t y, uint8_t color){
    for(uint8_t j = 0; j < BLOCK_HEIGHT; j++){
        for(uint8_t i = 0; i < BLOCK_WIDTH; i++){
            graphics_draw((BLOCK_WIDTH*x) + i, (BLOCK_HEIGHT*y) + j, color);
        }
    }
    return;
}

static cursor_t cursor = {
    .x = 0,
    .y = 0,
    .status = 0,
    .cursor_show = 0,
    .cached_bg = {0}
};
static uint8_t cursor_counter = 0;
static uint8_t cursor_blocker = 0;
static uint8_t cursor_blocker2 = 0;

uint8_t blocked_x = 0;
uint8_t blocked_y = 0;

static void cursor_callback(){
    if(!cursor_blocker && !cursor_blocker2){
        if(cursor_counter > 10){
            if (cursor.cursor_show){
                graphics_hide_cursor();
                cursor.cursor_show = 0;
            }
            else{
                graphics_show_cursor();
                cursor.cursor_show = 1;
            }
        }
        else{
            cursor_counter++;
        }
    }
    return;
}

void graphics_cursor_on(){
    activate_timer_interrupt(DEFAULT_FREQUENCY);
    register_interrupt_handler(32, cursor_callback);
    graphics_show_cursor();
    cursor.status = 1;
    return;
}

void graphics_cursor_off(){
    activate_timer_interrupt(DEFAULT_FREQUENCY);

    graphics_hide_cursor();
    cursor.cursor_show = 0;
    cursor.status = 0;

    return;
}

void graphics_show_cursor(){
    cursor_blocker = 1;
    cursor.cursor_show = 1;
    cursor_counter = 0;
    uint16_t xOffset = BLOCK_WIDTH*cursor.x;
    for(uint8_t j = 0; j < BLOCK_HEIGHT-1; j++){
        memcpy(&cursor.cached_bg[j], (MEMORY_GRAPHICS + (320 * (((BLOCK_HEIGHT)*cursor.y) + j)) + xOffset), 1);
        graphics_draw((BLOCK_WIDTH*cursor.x), ((BLOCK_HEIGHT)*cursor.y) + j, CURSOR_COLOR);
    }
    cursor_blocker = 0;
    return;
}

void graphics_hide_cursor(){
    cursor_blocker = 1;
    cursor.cursor_show = 0;
    cursor_counter = 0;
    for(uint8_t j = 0; j < BLOCK_HEIGHT-1; j++){
        graphics_draw((BLOCK_WIDTH*cursor.x), (BLOCK_HEIGHT*cursor.y) + j, cursor.cached_bg[j]);
    }
    cursor_blocker = 0;
    return;
}

uint8_t graphics_get_cursor_status(){
    return cursor.status;
}
uint8_t get_cursor_x(){
    return cursor.x;
}
uint8_t get_cursor_y(){
    return cursor.y;
}

void graphics_set_limit(uint8_t x, uint8_t y){
    blocked_x = x;
    blocked_y = y;
}

void graphics_set_cursor(uint8_t x, uint8_t y){
    cursor_blocker2 = 1;
    if(cursor.cursor_show){
        graphics_hide_cursor();
    }

    cursor.x = x;
    cursor.y = y;

    cursor.cursor_show = 1;
    graphics_show_cursor();
    cursor_blocker2 = 0;
    return;
}

uint8_t graphics_find_edge(uint8_t y){
    uint8_t counter = 0;
    while (screenMap[y][counter] != 0 && counter < colLimit){
        counter++;
    }
    return counter;
}

bool graphics_move_cursor(int8_t direction){
    bool success = 0;
    switch (direction){
        case (1):
            if((cursor.x == 63 && screenMap[cursor.y][cursor.x] != 0) && cursor.y != 24){
                graphics_set_cursor(0, cursor.y + 1);
                success = 1;
            }
            else{
                if(screenMap[cursor.y][cursor.x] != 0){
                    graphics_set_cursor(cursor.x + 1,cursor.y);
                    success = 1;
                }
            }
            break;
        case (-1):
            if(!(cursor.y == blocked_y && cursor.x == blocked_x)){
                if(cursor.x != 0 ){
                    graphics_set_cursor(cursor.x - 1, cursor.y);
                    success = 1;
                }
                else{
                    if(cursor.y != 0){
                        graphics_set_cursor(graphics_find_edge(cursor.y-1), cursor.y-1);
                        success = 1;
                    }
                }
            }
            break;
        
        default:
            success = 0;
            break;
    }
    return success;
}

void graphics_write_char_c(uint8_t x, uint8_t y, char c, uint8_t color){
    uint8_t index = (uint8_t) c;
    uint16_t blockStartX = x*BLOCK_WIDTH;
    uint16_t blockStartY = y*BLOCK_HEIGHT;

    for(int i = 1; i < lookup[index][0]; i++){
        graphics_draw(blockStartX + (lookup[index][i] >> 4), blockStartY + (lookup[index][i] & 0x0f), color);
    }

    return;
}

void graphics_write_char(char c){

    if(c != '\n'){
        if(cursor.x == colLimit && cursor.y == rowLimit-1){
            graphics_scroll();
            graphics_set_cursor(0, rowLimit - 1);
        }

        uint16_t end = colLimit*rowLimit;
        uint16_t loc = cursor.y*colLimit + cursor.x;
        for(int i = end-1; i >= loc; i--){
            screenMap[0][i] = screenMap[0][i - 1];
        }

        screenMap[cursor.y][cursor.x] = c;
        graphics_move_cursor(1);
        refresh_screen_buffer();
    }
    else{
        if(cursor.y == rowLimit -1){
            graphics_scroll();
            graphics_set_cursor(0, rowLimit - 1);
        }
        else{
            graphics_set_cursor(0, cursor.y+1);
        }
    }


    return;
}

void refresh_screen_buffer(){
    cursor_blocker2 = 1;
    graphics_clear();
    for(int i = 0; i < rowLimit; i++){
        for(int j = 0; j < colLimit; j++){
            if(screenMap[i][j] != 0){
                graphics_write_char_c(j, i, screenMap[i][j], DEFAULT_COLOR_FG);
            }
        }
    }
    graphics_show_cursor();
    cursor_blocker2 = 0;
}

void graphics_scroll(){
    for(int i = 0; i < rowLimit-1; i++){
        memcpy(screenMap[i], screenMap[i+1], colLimit);
    }
    memset(screenMap[rowLimit-1], 0, colLimit);
}

void graphics_print(char* string){
    //kurang sangkil, masih bisa diimprove
    int i = 0;
    while (string[i] != 0){
        if(string[i] != '\n'){
            if(cursor.x == colLimit && cursor.y == rowLimit-1){
                graphics_scroll();
                graphics_set_cursor(0, rowLimit - 1);
            }

            uint16_t end = colLimit*rowLimit;
            uint16_t loc = cursor.y*colLimit + cursor.x;
            for(int i = end-1; i >= loc; i--){
                screenMap[0][i] = screenMap[0][i - 1];
            }

            screenMap[cursor.y][cursor.x] = string[i];
            graphics_move_cursor(1);
        }
        else{
            if(cursor.y == rowLimit -1){
                graphics_scroll();
                graphics_set_cursor(0, rowLimit - 1);
            }
            else{
                graphics_set_cursor(0, cursor.y+1);
            }
        }
        i++;
    }
    refresh_screen_buffer();

    return;
}

bool graphics_backspace(){
    bool success = 0;

    if(!(cursor.y == blocked_y && cursor.x == blocked_x)){
        uint16_t end = colLimit*rowLimit;
        uint16_t loc = cursor.y*colLimit + cursor.x;
        for(int i = loc-1; i < end; i++){
            screenMap[0][i] = screenMap[0][i + 1];
        }
        screenMap[0][end-1] = 0;
        success = 1;
    }
    else{
        success = 0;
    }

    if(success){
        graphics_move_cursor(-1);
        refresh_screen_buffer();
    }
    

    return success;
}