#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "lib-header/stdtype.h"

#define KEYBOARD_BUFFER_SIZE 1000

struct keyboard{
    char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
    uint16_t maxIdx;
    uint16_t currentIdx;
    uint8_t caps;
    uint8_t shift;
    uint8_t keyboard_input_on;
};
typedef struct keyboard keyboardDriverState;

void activate_keyboard_interrupt();
void keyboard_state_activate();
void keyboard_state_deactivate();
void clear_reader();
void append_reader(char in);
void move_reader(int direction);
void backspace_reader();
char* get_keyboard_buffer();
void keyboardDriver(uint8_t);

#endif