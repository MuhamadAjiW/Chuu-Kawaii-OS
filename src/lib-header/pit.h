#ifndef _TIMER_H
#define _TIMER_H

#include "stdtype.h"

#define DEFAULT_FREQUENCY 20

/**
 *  Call assigned interrupt response
 * 
 */
void pit_callback();

/**
 *  Assign default interrupt response
 * 
 *  @param freq frequency of timer, should be larger than 18 and smaller then 10000 or is going to be unstable
 */
void activate_pit_interrupt(uint32_t freq);


/**
 *  Assign default interrupt response
 * 
 *  @param duration duration of sleep in ticks, ticks is generated based on the set frequency
 */
void sleep(uint32_t duration);

/**
 *  Returns current tick since default timer interrupt is activated
 * 
 *  @return current tick since default timer interrupt is activated
 */
uint32_t get_tick();

#endif