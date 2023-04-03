
#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "stdtype.h"

void dir();
void clear();

uint8_t move_cursor(int direction);
uint8_t backspace();

#endif