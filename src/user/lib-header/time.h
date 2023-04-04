
#ifndef _TIME_H
#define _TIME_H

#include "stdtype.h"

struct time
{
    uint8_t century;
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
};
typedef struct time time;

time get_time();

#endif