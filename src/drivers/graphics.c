#include "../lib-header/graphics.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/string.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/portio.h"
#include "../lib-header/pit.h"
#include "../lib-header/isr.h"
#include "../lib-header/stdfont.h"
#include "../lib-header/stdbg.h"

void initialize_vga(){
    //source
    //https://wiki.osdev.org/VGA_Hardware#Memory_Layout_in_256-color_graphics_modes
    //http://www.osdever.net/FreeVGA/home.htm

    out(MISC_OUT_REG, 0x63);
    out2(CRT_ADDR_REG, (uint16_t) ( (0x0e << 8) + 0x11));

    out2(CRT_ADDR_REG, (uint16_t) ( (0x5f << 8) + 0x0));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x4f << 8) + 0x1));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x50 << 8) + 0x2));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x82 << 8) + 0x3));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x54 << 8) + 0x4));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x80 << 8) + 0x5));
    out2(CRT_ADDR_REG, (uint16_t) ( (0xbf << 8) + 0x6));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x1f << 8) + 0x7));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x00 << 8) + 0x8));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x41 << 8) + 0x9));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x9c << 8) + 0x10));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x8e << 8) + 0x11));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x8f << 8) + 0x12));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x28 << 8) + 0x13));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x40 << 8) + 0x14));
    out2(CRT_ADDR_REG, (uint16_t) ( (0x96 << 8) + 0x15));
    out2(CRT_ADDR_REG, (uint16_t) ( (0xb9 << 8) + 0x16));
    out2(CRT_ADDR_REG, (uint16_t) ( (0xa3 << 8) + 0x17));

    out2(MEM_MODE_REG, (uint16_t) ( (0x01 << 8) + 0x01));
    out2(MEM_MODE_REG, (uint16_t) ( (0x0f << 8) + 0x02));
    out2(MEM_MODE_REG, (uint16_t) ( (0x00 << 8) + 0x03));
    out2(MEM_MODE_REG, (uint16_t) ( (0x0e << 8) + 0x04));

    out2(GRAP_ADDR_REG, (uint16_t) ( (0x40 << 8) + 0x05));
    out2(GRAP_ADDR_REG, (uint16_t) ( (0x05 << 8) + 0x06));

    in(INPUT_STATUS_1);
    out(ATTR_ADDR_REG,0x10);
    out2(ATTR_ADDR_REG,0x41);

    in(INPUT_STATUS_1);
    out(ATTR_ADDR_REG,0x11);
    out2(ATTR_ADDR_REG,0x00);

    in(INPUT_STATUS_1);
    out(ATTR_ADDR_REG,0x12);
    out2(ATTR_ADDR_REG,0x0f);
    
    in(INPUT_STATUS_1);
    out(ATTR_ADDR_REG,0x13);
    out2(ATTR_ADDR_REG,0x00);

    in(INPUT_STATUS_1);
    out(ATTR_ADDR_REG,0x14);
    out2(ATTR_ADDR_REG,0x00);

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
        28,  0,  0,  28,  7,  0,  25, 21, 12, 10, 13, 5, 
        28, 20, 11, 30, 27, 14, 17, 19, 11, 8, 1, 8, 
        17, 12, 22, 10, 6, 5, 17, 24, 27, 10, 8, 14, 
        11, 7, 11, 6, 6, 18, 20, 14, 19, 8, 10, 22, 
        7, 5, 4, 15, 11, 16, 23, 15, 22, 23, 26, 32, 
        19, 12, 15, 31, 22, 28, 24, 18, 26, 17, 4, 12, 
        2, 5, 13, 20, 21, 28, 28, 22, 32, 21, 8, 16, 
        12, 7, 20, 10, 4, 10, 9, 6, 17, 10, 9, 17, 
        8, 12, 19, 24, 10, 22, 14, 4, 12, 11, 14, 21, 
        7, 18, 21, 20, 15, 22, 22, 9, 19, 18, 8, 17, 
        29, 11, 23, 19, 11, 19, 7, 13, 16, 27, 22, 22, 
        25, 28, 28, 31, 28, 28, 26, 28, 33, 14, 7, 12, 
        16, 19, 24, 4, 11, 15, 2, 8, 12, 27, 15, 22, 
        20, 24, 29, 20, 27, 28, 24, 19, 21, 18, 11, 11, 
        28, 20, 26, 11, 16, 18, 28, 26, 26, 22, 15, 15, 
        16, 21, 27, 13, 18, 23, 13, 27, 30, 15, 9, 8, 
        26, 25, 29, 11, 23, 27, 9, 21, 24, 16, 30, 31, 
        30, 15, 22, 16, 27, 29, 14, 25, 26, 33, 27, 31, 
        19, 31, 31, 7, 15, 19, 10, 17, 21, 20, 28, 31, 
        23, 30, 31, 9, 5, 14, 22, 18, 18, 13, 21, 24, 
        26, 31, 32, 24, 22, 28, 15, 19, 20, 17, 22, 24, 
        5, 9, 11, 19, 15, 15, 23, 25, 29, 3, 6, 9, 
        17, 14, 18, 7, 10, 14, 21, 12, 22, 6, 1, 4, 
        15, 14, 14, 20, 20, 24, 14, 16, 20, 6, 3, 11, 
        27, 19, 23, 11, 0, 5, 28, 21, 29, 28, 18, 29, 
        0, 2, 6, 25, 15, 28, 21, 12, 25, 5, 2, 8, 
        31, 25, 28, 14, 12, 19, 21, 16, 25, 16, 9, 20, 
        13, 8, 16, 29, 31, 30, 17, 16, 21, 3, 0, 1, 
        11, 13, 17, 1, 3, 10, 7, 9, 19, 15, 4, 9, 
        11, 10, 10, 21, 23, 25, 23, 14, 18, 18, 19, 21, 
        3, 5, 18, 24, 22, 23, 28, 23, 25, 11, 3, 7, 
        18, 7, 12, 26, 13, 19, 22, 11, 16, 5, 7, 14, 
        19, 17, 18, 28, 28, 29, 0, 0, 0, 25, 25, 26, 
        10, 11, 14, 6, 6, 10, 10, 7, 8, 13, 11, 13, 
        21, 20, 21, 32, 30, 32, 3, 2, 4, 7, 5, 7, 
        8, 9, 11, 4, 5, 6, 14, 15, 17, 0, 1, 3
    };

    out(DAC_WRITE_REG, 0);
    for(int i = 0; i < 768; i += 3){
        out(DAC_DATA_REG, (uint8_t) paletteArray[i]);
        out(DAC_DATA_REG, (uint8_t) paletteArray[i + 1]);
        out(DAC_DATA_REG, (uint8_t) paletteArray[i + 2]);
    }

    out(ATTR_ADDR_REG, 0x20);
    return;
}

uint8_t colLimit = SCREEN_WIDTH/BLOCK_WIDTH;
uint8_t rowLimit = SCREEN_HEIGHT/BLOCK_HEIGHT;
char screenMap[SCREEN_HEIGHT/BLOCK_HEIGHT][SCREEN_WIDTH/BLOCK_WIDTH] = {0};
char screenMapColor[SCREEN_HEIGHT/BLOCK_HEIGHT][SCREEN_WIDTH/BLOCK_WIDTH] = {0};
uint8_t backBuffer[320*200];

void clear_graphics_memory(){
    memset(MEMORY_GRAPHICS, DEFAULT_COLOR_BG, 0xfa00);
    return;
}

void graphics_clear(){
    memcpy(backBuffer, background, 0xfa00);
    return;
}

void graphics_swap_buffer(){
    memcpy(MEMORY_GRAPHICS, backBuffer, 0xfa00);
    return;
}

void graphics_clear_buffer(){
    for(int i = 0; i < rowLimit; i++){
        for(int j = 0; j < colLimit; j++){
            screenMap[i][j] = 0;
            screenMapColor[i][j] = 0;
        }
    }
    graphics_clear();
    graphics_set_cursor(0, 0);
    return;
}

void graphics_draw(uint16_t x, uint16_t y, uint8_t color){
    memset(&backBuffer[(320 * y) + x], color, 1);
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
    pit_callback();
    return;
}

void graphics_cursor_on(){
    activate_pit_interrupt(DEFAULT_FREQUENCY);
    register_interrupt_handler(32, cursor_callback);
    graphics_show_cursor();
    cursor.status = 1;
    return;
}

void graphics_cursor_off(){
    activate_pit_interrupt(DEFAULT_FREQUENCY);

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
        memcpy(&cursor.cached_bg[j], (&backBuffer[(320 * (((BLOCK_HEIGHT)*cursor.y) + j)) + xOffset]), 1);
        graphics_draw((BLOCK_WIDTH*cursor.x), ((BLOCK_HEIGHT)*cursor.y) + j, CURSOR_COLOR);
    }
    graphics_swap_buffer();
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
    graphics_swap_buffer();
    cursor_blocker = 0;
    return;
}

uint8_t graphics_get_cursor_status(){
    return cursor.status;
}
uint8_t graphics_get_cursor_x(){
    return cursor.x;
}
uint8_t graphics_get_cursor_y(){
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
    if(counter == BUFFER_WIDTH) counter--;
    
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
    graphics_write_char_color(c, DEFAULT_COLOR_FG);
    return;
}

void graphics_write_char_color(char c, uint8_t color){
    if(c != '\n'){
        if(cursor.x == colLimit && cursor.y == rowLimit-1){
            graphics_scroll();
            graphics_set_cursor(0, rowLimit - 1);
        }

        uint16_t end = colLimit*rowLimit;
        uint16_t loc = cursor.y*colLimit + cursor.x;
        for(int i = end-1; i >= loc; i--){
            screenMap[0][i] = screenMap[0][i - 1];
            screenMapColor[0][i] = screenMapColor[0][i-1];
        }

        screenMap[cursor.y][cursor.x] = c;
        screenMapColor[cursor.y][cursor.x] = color;
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
                graphics_write_char_c(j, i, screenMap[i][j], screenMapColor[i][j]);
            }
        }
    }
    graphics_show_cursor();
    graphics_swap_buffer();
    cursor_blocker2 = 0;
}

void graphics_scroll(){
    for(int i = 0; i < rowLimit-1; i++){
        memcpy(screenMap[i], screenMap[i+1], colLimit);
        memcpy(screenMapColor[i], screenMapColor[i+1], colLimit);
    }
    memset(screenMap[rowLimit-1], 0, colLimit);
    memset(screenMapColor[rowLimit-1], 0, colLimit);
}

void graphics_print(char* string){
    graphics_print_color(string, DEFAULT_COLOR_FG);
    return;
}

void graphics_print_color(char* string, uint8_t color){
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
                screenMapColor[0][i] = screenMapColor[0][i - 1];
            }

            screenMap[cursor.y][cursor.x] = string[i];
            screenMapColor[cursor.y][cursor.x] = color;
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
            screenMapColor[0][i] = screenMapColor[0][i + 1];
        }
        screenMap[0][end-1] = 0;
        screenMapColor[0][end-1] = 0;
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