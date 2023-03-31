#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "../lib-header/stdtype.h"

/* VGA Port Macros */
#define MISC_OUT_REG 0x3c2
#define FEAT_CONT_REG 0x3ca

#define ATTR_ADDR_REG 0x3c0
#define ATTR_DATA_REG 0x3c1

#define CRT_ADDR_REG 0x3d4
#define CRT_DATA_REG 0x3d5

#define MEM_MODE_REG 0x3c4

#define DAC_MASK_REG 0x3c6
#define DAC_READ_REG 0x3c7
#define DAC_WRITE_REG 0x3c8
#define DAC_DATA_REG 0x3c9
#define DAC_STATE_REG 0x3c7

#define GRAP_ADDR_REG 0x3ce
#define GRAP_DATA_REG 0x3cf

#define INPUT_STATUS_1 0x3da

/* Graphics Attributes */
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200


#define BLOCK_WIDTH 5
#define BLOCK_HEIGHT 8

#define BUFFER_HEIGHT 25
#define BUFFER_WIDTH 64


#define DEFAULT_COLOR_BG 84
#define DEFAULT_COLOR_FG 15
#define CURSOR_COLOR 13

#define MEMORY_GRAPHICS (uint8_t *) 0xC00A0000 //0xA0000 sebelum remapping

struct cursor_t{
    uint8_t x;
    uint8_t y;
    uint8_t status;
    uint8_t cursor_show;
    uint8_t cached_bg[8];
};
typedef struct cursor_t cursor_t;

void initialize_vga();

void graphics_clear();
void graphics_swap_buffer();
void graphics_clear_buffer();
void graphics_draw(uint16_t x, uint16_t y, uint8_t color);
void graphics_draw_block(uint8_t x, uint8_t y, uint8_t color);

void graphics_cursor_on();
void graphics_cursor_off();
void graphics_show_cursor();
void graphics_hide_cursor();

uint8_t get_cursor_status();
uint8_t get_cursor_x();
uint8_t get_cursor_y();
void graphics_set_limit(uint8_t, uint8_t);

void graphics_set_cursor(uint8_t x, uint8_t y);
uint8_t graphics_find_edge(uint8_t y);
bool graphics_move_cursor(int8_t direction);

void graphics_write_char_c(uint8_t x, uint8_t y, char c, uint8_t color);
void graphics_write_char(char c);
void refresh_screen_buffer();
void graphics_scroll();
void graphics_print(char* string);
bool graphics_backspace();

#endif