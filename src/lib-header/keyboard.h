#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "stdtype.h"

#define NULL_CHAR 0
#define TAB_CHAR 1
#define LARROW_CHAR 2
#define RARROW_CHAR 3
#define BACKSPACE_CHAR 4

struct keyboard{
    char last;
    //char* keyboard_buffer;
    //uint32_t buffersize;
    //uint16_t maxIdx;
    //uint16_t currentIdx;
    uint8_t caps;
    uint8_t shift;
    uint8_t keyboard_input_on;
};
typedef struct keyboard keyboardDriverState;

void activate_keyboard_interrupt();
void keyboard_state_activate();
void keyboard_state_deactivate();
//void clear_reader();
//void append_reader(char in);
//void move_reader(int direction);
//void backspace_reader();
//char* get_keyboard_buffer();

char get_keyboard_last_key();
void set_keyboard_last_key(char in);
void keyboard_driver(uint8_t);
void keyboard_driver_graphics(uint8_t input);

#endif