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
        63, 31, 31,  63, 39, 31,  63, 47, 31,  51, 44, 20, 
        20, 26, 8, 58, 41, 19, 60, 56, 23, 34, 39, 19, 
        14, 17, 15, 12, 25, 39, 57, 58, 63, 16, 7, 8, 
        28, 35, 49, 46, 51, 49, 46, 56, 61, 32, 13, 14, 
        33, 19, 24, 2, 3, 16, 25, 17, 27, 17, 20, 46, 
        40, 23, 29, 14, 12, 35, 39, 26, 35, 49, 34, 45, 
        26, 14, 19, 7, 7, 26, 30, 24, 30, 18, 13, 24, 
        8, 7, 18, 38, 36, 54, 53, 36, 53, 55, 50, 62, 
        28, 13, 33, 55, 45, 63, 46, 46, 62, 50, 44, 57, 
        18, 28, 43, 30, 18, 34, 26, 15, 39, 37, 38, 49, 
        14, 6, 27, 42, 24, 40, 17, 24, 38, 41, 29, 45, 
        47, 21, 45, 23, 11, 24, 29, 9, 25, 28, 34, 44, 
        23, 28, 41, 33, 19, 29, 58, 24, 48, 44, 18, 39, 
        38, 19, 35, 23, 23, 36, 37, 25, 40, 61, 45, 53, 
        50, 40, 42, 13, 20, 33, 18, 17, 29, 12, 20, 27, 
        1, 11, 21, 47, 54, 56, 62, 56, 56, 23, 35, 46, 
        40, 43, 53, 46, 51, 62, 30, 14, 25, 60, 23, 43, 
        8, 12, 24, 4, 17, 25, 53, 45, 47, 48, 37, 37, 
        49, 22, 38, 14, 2, 12, 22, 40, 49, 63, 49, 63, 
        26, 54, 60, 41, 49, 59, 33, 32, 44, 39, 54, 56, 
        38, 22, 22, 31, 43, 57, 58, 39, 53, 23, 33, 36, 
        8, 1, 1, 51, 55, 63, 32, 40, 51, 17, 39, 47, 
        57, 50, 52, 31, 18, 17, 57, 30, 48, 22, 47, 56, 
        18, 23, 33, 54, 25, 43, 13, 34, 42, 22, 45, 50, 
        30, 60, 63, 44, 30, 32, 62, 30, 46, 28, 39, 43, 
        31, 54, 59, 10, 25, 31, 2, 9, 16, 16, 12, 30, 
        23, 13, 13, 36, 60, 62, 14, 30, 37, 29, 48, 54, 
        42, 62, 62, 22, 13, 34, 32, 24, 25, 37, 49, 54, 
        20, 34, 41, 41, 57, 62, 18, 30, 31, 43, 37, 37, 
        13, 0, 7, 49, 43, 52, 27, 43, 48, 48, 61, 63, 
        54, 63, 63, 39, 28, 28, 35, 45, 48, 10, 18, 21, 
        23, 12, 29, 49, 32, 40, 52, 51, 57, 47, 34, 50, 
        39, 32, 33, 7, 12, 19, 42, 39, 48, 42, 24, 45, 
        39, 10, 24, 63, 56, 61, 39, 35, 43, 28, 32, 39, 
        61, 43, 59, 13, 6, 22, 55, 39, 46, 22, 0, 11, 
        56, 41, 58, 56, 36, 58, 52, 31, 56, 43, 25, 50, 
        63, 51, 58, 19, 6, 13, 45, 48, 54, 23, 28, 36, 
        29, 25, 39, 42, 31, 50, 32, 18, 40, 34, 30, 38, 
        25, 18, 32, 9, 2, 13, 35, 40, 44, 41, 44, 48, 
        3, 6, 21, 15, 18, 41, 32, 8, 19, 24, 24, 31, 
        21, 20, 21, 50, 27, 35, 9, 14, 34, 7, 9, 40, 
        7, 3, 6, 33, 36, 39, 24, 6, 15, 40, 15, 25, 
        55, 26, 38, 48, 21, 33, 10, 8, 8, 11, 14, 29, 
        38, 35, 38, 57, 55, 58, 0, 3, 11, 33, 29, 31, 
        52, 51, 52, 19, 22, 28, 1, 4, 1, 13, 13, 24, 
        12, 12, 19, 21, 15, 18, 46, 45, 46, 4, 8, 10, 
        25, 22, 26, 44, 40, 42, 63, 62, 63, 14, 10, 14, 
        16, 18, 21, 9, 10, 13, 28, 31, 34, 2, 3, 6, 
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
    graphics_swap_buffer();
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