
#ifndef _DISK_H
#define _DISK_H

#include "stdtype.h"

#define READ_CODE 32
#define WRITE_CODE 48

/**
 * Read blocks from the hard drive
 * Direct use is discouraged and it is better to use read_clusters from fat32 
 * 
 * @param target_address    address of the result
 * @param lba               linear block address, basically disk address in 512 clusters
 * @param sector_count      number of sectors to read
*/
void read_blocks(uint32_t* target_address, uint32_t lba, uint8_t sector_count);

/**
 * Write blocks to the hard drive
 * Direct use is discouraged and it is better to use write_clusters from fat32 
 * 
 * @param target_address    address of bytes to write
 * @param lba               linear block address, basically disk address in 512 clusters
 * @param sector_count      number of sectors to write
*/
void write_blocks(uint32_t* entry, uint32_t lba, uint8_t sector_count);

#endif