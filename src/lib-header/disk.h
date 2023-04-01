
#ifndef _DISK_H
#define _DISK_H

#include "stdtype.h"

#define READ_CODE 32
#define WRITE_CODE 48

void disk_callback();
void activate_disk_interrupt();
void read_blocks(uint32_t* target_address, uint32_t lba, uint8_t sector_count);
void write_blocks(uint32_t lba, uint8_t sector_count, uint32_t* entry);

#endif