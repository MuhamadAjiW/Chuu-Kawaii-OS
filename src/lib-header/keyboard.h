#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "stdtype.h"

#define NULL_CHAR 0
#define TAB_CHAR 1
#define LARROW_CHAR 2
#define RARROW_CHAR 3
#define UARROW_CHAR 4
#define DARROW_CHAR 5
#define ESC_CHAR 6
#define BACKSPACE_CHAR 7

/**
 *  Singleton struct to store necessary keyboard information
 *  
 *  @param last                 last key pressed on the keyboard
 *  @param caps                 capslock status on the keyboard
 *  @param shift                shift-pressed status on the keyboard
 *  @param keyboard_input_on    Keyboard callback activation flag
 * 
 *  @return address of allocated memory
 */
struct keyboard{
    char last;
    uint8_t caps;
    uint8_t shift;
    uint8_t keyboard_input_on;
};
typedef struct keyboard keyboardDriverState;

/**
 *  Assign keyboard callback function to interrupt handler
 * 
 */
void activate_keyboard_interrupt();


/**
 *  Set keyboard activation flag to 1
 * 
 */
void keyboard_state_activate();

/**
 *  Set keyboard activation flag to 0
 * 
 */
void keyboard_state_deactivate();

/**
 *  Return keyboard struct last pressed key
 *  
 *  @return keyboard last pressed key
 */
char get_keyboard_last_key();


/**
 *  Set keyboard struct last pressed key
 * 
 */
void set_keyboard_last_key(char in);


/**
 *  Default callback for keyboard interrupts
 *  
 *  @param input received value from keyboard port
 * 
 */
void keyboard_driver_graphics(uint8_t input);

#endif