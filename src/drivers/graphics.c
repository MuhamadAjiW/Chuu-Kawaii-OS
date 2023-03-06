#include "../lib-header/graphics.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/portio.h"
#include "../lib-header/timer.h"
#include "../lib-header/isr.h"
#include "lib-header/stdfont.h"

void initialize_vga(){
    //intinya switch ke mode 13h, tapi gatau kenapa 64 color doang, harus akses register colornya tapi masih gak ngerti :(

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

    out(0x3c0, 0x20);
    return;
}

uint8_t colLimit = SCREEN_WIDTH/BLOCK_WIDTH;
uint8_t rowLimit = SCREEN_HEIGHT/BLOCK_HEIGHT;
char screenMap[SCREEN_HEIGHT/BLOCK_HEIGHT][SCREEN_WIDTH/BLOCK_WIDTH] = {0};


void graphics_clear(){
    memset(MEMORY_GRAPHICS, 47, 0x10000);
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

void graphics_set_cursor(uint8_t x, uint8_t y){
    cursor_blocker2 = 1;
    graphics_show_cursor();
    graphics_hide_cursor();

    cursor.x = x;
    cursor.y = y;

    cursor.cursor_show = 1;
    graphics_show_cursor();
    cursor_blocker2 = 0;
    return;
}

uint8_t graphics_find_edge(uint8_t y){
    uint8_t counter = 0;
    while (screenMap[y][counter] != 0 && counter < rowLimit){
        counter++;
    }
    return counter;
}

bool graphics_move_cursor(int8_t direction){
    bool success = 0;
    switch (direction){
        case (1):
            if((cursor.x == 63 && screenMap[cursor.y][cursor.x + 1] != 0) && cursor.y != 24){
                graphics_set_cursor(0, cursor.y + 1);
                success = 1;
            }
            else{
                if(screenMap[cursor.y][cursor.x + 1] != 0){
                    graphics_set_cursor(cursor.x + 1,cursor.y);
                    success = 1;
                }
            }
            break;
        case (-1):
            if(!(cursor.y == blocked_y && cursor.x == blocked_x + 1)){
                if(cursor.x != 0){
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

void graphics_write(uint8_t x, uint8_t y, char c, uint8_t color){
    uint8_t index = (uint8_t) c;
    uint16_t blockStartX = x*BLOCK_WIDTH;
    uint16_t blockStartY = y*BLOCK_HEIGHT;

    for(int i = 1; i < lookup[index][0]; i++){
        graphics_draw(blockStartX + (lookup[index][i] >> 4), blockStartY + (lookup[index][i] & 0x0f), color);
    }
    screenMap[x][y] = c;

    return;
}
