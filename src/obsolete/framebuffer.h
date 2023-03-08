#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include "lib-header/stdtype.h"

#define MEMORY_FRAMEBUFFER (uint8_t *) 0xB8000
#define CURSOR_PORT_CMD    0x03D4
#define CURSOR_PORT_DATA   0x03D5

/**
 * Terminal framebuffer
 * Resolution: 80x25
 * Starting at MEMORY_FRAMEBUFFER,
 * - Even number memory: Character, 8-bit
 * - Odd number memory:  Character color lower 4-bit, Background color upper 4-bit
*/

/**
 * Set framebuffer character and color with corresponding parameter values.
 * More details: https://en.wikipedia.org/wiki/BIOS_color_attributes
 *
 * @param row Vertical location (index start 0)
 * @param col Horizontal location (index start 0)
 * @param c   Character
 * @param fg  Foreground / Character color
 * @param bg  Background color
 */
void framebuffer_write(uint8_t row, uint8_t col, char c, uint8_t fg, uint8_t bg);

/**
 * Set cursor to specified location. Row and column starts from 0
 * 
 * @param r row
 * @param c column
*/
void framebuffer_set_cursor(uint8_t r, uint8_t c);

/** 
 * Set all cell in framebuffer character to 0x00 (empty character)
 * and color to 0x07 (gray character & black background)
 * 
 */
void framebuffer_clear(void);



//extras
void clear_video_memory();
void framebuffer_set_limit(uint8_t r, uint8_t c);
void framebuffer_print(char* string, uint8_t fg, uint8_t bg);
void framebuffer_printDef(char* string);
void framebuffer_insert_char(char in);
bool framebuffer_backspace();
void framebuffer_scroll();
int framebuffer_find_edge(int row);
int framebuffer_find_edge_whole(int row, int col);
uint16_t framebuffer_get_cursor();
bool framebuffer_move_cursor(int direction);
void int_toString(int x, char str[]);
int strlen(char str[]);

int limitCol;
int limitRow;

#endif