#ifndef _TIMER_H
#define _TIMER_H

#include "lib-header/stdtype.h"

void activate_timer_interrupt(uint32_t);
void sleep(uint32_t duration);

#endif