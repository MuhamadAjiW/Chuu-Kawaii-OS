#ifndef _TIMER_H
#define _TIMER_H

#include "lib-header/stdtype.h"

#define DEFAULT_FREQUENCY 20

void pit_callback();
void activate_pit_interrupt(uint32_t);
void sleep(uint32_t duration);
uint32_t get_tick();

#endif