#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "stdtype.h"

/* further reading: 
    https://wiki.osdev.org/VGA_Hardware#Memory_Layout_in_256-color_graphics_modes
    http://www.osdever.net/FreeVGA/home.htm
*/

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

/**
 *  Singleton struct for cursor information
 *  @param x            horizontal coordinate of cursor in blocks, not in pixel
 *  @param y            vertical coordinate of cursor in blocks, not in pixel
 *  @param status       activation status of cursor, 1 means cursor will continuously blink
 *  @param cursor_show  showing status of cursor, 1 means cursor is shown, 0 means cursor is hidden
 *  @param cached_bg    saved background information for hidden cursors
 * 
 */
struct cursor_t{
    uint8_t x;
    uint8_t y;
    uint8_t status;
    uint8_t cursor_show;
    uint8_t cached_bg[8];
};
typedef struct cursor_t cursor_t;

/**
 * Configures VGA to use mode 13h
 * Also loads necessary palette for the system
 * 
 */
void initialize_vga();


//TODO: Document
void clear_graphics_memory();
void graphics_display_animation();
void graphics_user_animation(int framecode);

/**
 * Overwrites every pixel with current given background image
 * 
 */
void graphics_clear();

/**
 * Overwrites shown screen with writable buffer
 * 
 */
void graphics_swap_buffer();

/**
 * Clears writable buffer
 * 
 */
void graphics_clear_buffer();

/**
 * Sets 1 pixel of screen to a certain color
 * 
 * @param x         horizontal coordinate of pixel
 * @param y         vertical coordinate of pixel
 * @param color     index of color palette
 */
void graphics_draw(uint16_t x, uint16_t y, uint8_t color);

/**
 * Sets 1 block of screen to a certain color
 * 
 * @param x         horizontal coordinate of block
 * @param y         vertical coordinate of block
 * @param color     index of color palette
 */
void graphics_draw_block(uint8_t x, uint8_t y, uint8_t color);

/**
 * Sets cursor_show flag of cursor to 1
 * 
 */
void graphics_cursor_on();

/**
 * Sets cursor_show flag of cursor to 0
 * 
 */
void graphics_cursor_off();

/**
 * Shows cursor at current coordinates
 * 
 */
void graphics_show_cursor();

/**
 * Hides cursor
 * 
 */
void graphics_hide_cursor();

/**
 * @return cursor status value
 * 
 */
uint8_t graphics_get_cursor_status();

/**
 * @return cursor x value
 * 
 */
uint8_t graphics_get_cursor_x();

/**
 * @return cursor y value
 * 
 */
uint8_t graphics_get_cursor_y();

/**
 * blocks cursor at a certain coordinate
 * 
 * @param x         horizontal coordinate of blocker (in blocks)
 * @param y         vertical coordinate of blocker (in blocks)
 */
void graphics_set_limit(uint8_t x, uint8_t y);

/**
 * moves cursor to a certain coordinate
 * 
 * @param x         horizontal coordinate of blocker (in blocks)
 * @param y         vertical coordinate of blocker (in blocks)
 */
void graphics_set_cursor(uint8_t x, uint8_t y);

/**
 * find the x coordinate of last valid char in a line
 * 
 * @param y line
 * 
 * @return x value
 */
uint8_t graphics_find_edge(uint8_t y);

/**
 * moves cursor to a certain direction
 * 
 * @param direction moving direction, 1 for right, -1 for left
 * 
 * @return success, if coordinate is blocked or is an invalid char then return 0, else return 1
 */
bool graphics_move_cursor(int8_t direction);

/**
 * Sets 1 block of screen to a certain char
 * 
 * @param x         horizontal coordinate of block
 * @param y         vertical coordinate of block
 * @param c         written char
 * @param color     index of color palette
 */
void graphics_write_char_c(uint8_t x, uint8_t y, char c, uint8_t color);

/**
 * Assigns char with default colors at current cursor location to writable buffer
 * 
 * @param c         written char
 */
void graphics_write_char(char c);

/**
 * Assigns char with default colors at current cursor location to writable buffer
 * 
 * @param c         written char
 * @param color     index of color palette
 */
void graphics_write_char_color(char c, uint8_t color);

/**
 * Writes written characters to writable buffer then copies it into actual shown screen
 * 
 */
void refresh_screen_buffer();

/**
 * Copies all letters 1 block upwards and creates a clear line in the bottom
 * 
 */
void graphics_scroll();

/**
 * Assigns null terminated string with default colors at current cursor location to writable buffer
 * @warning         Do not use on non-null-terminated strings
 * 
 * @param string    written string
 */
void graphics_print(char* string);

/**
 * Writes null terminated string with user-defined colors at current cursor location
 * @warning         Do not use on non-null-terminated strings
 * 
 * @param string    written string
 * @param color     index of color palette
 */
void graphics_print_color(char* string, uint8_t color);

/**
 * Deletes a char on the left of the cursor
 * 
 * @return success, if coordinate is blocked or is an invalid char then return 0, else return 1
 */
bool graphics_backspace();

#endif