#ifndef _TIMER_H
#define _TIMER_H

#include "lib-header/stdtype.h"

#define DEFAULT_FREQUENCY 20

void activate_timer_interrupt(uint32_t);
void sleep(uint32_t duration);
uint32_t get_tick();

#endif