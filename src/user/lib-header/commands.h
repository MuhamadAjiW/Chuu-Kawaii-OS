
#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "stdtype.h"
#include "stdio.h"

void dir();
void clear();

uint8_t move_cursor(int direction);
uint8_t backspace();

uint8_t is_empty_empty(DirectoryEntry in);
void dir(uint32_t currentCluster);

#endif