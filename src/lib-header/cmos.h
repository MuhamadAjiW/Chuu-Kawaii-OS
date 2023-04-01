#ifndef _CMOS_H
#define _CMOS_H

#include "stdtype.h"

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

#define TIMEZONE 7

struct cmos_reader{
    uint8_t century;
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
};
typedef struct cmos_reader cmos_reader;

void initialize_cmos();
bool update_in_progress();
uint8_t get_reg(int reg);
void read_rtc();
cmos_reader get_cmos_data();

#endif