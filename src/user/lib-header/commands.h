
#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "stdtype.h"
#include "stdio.h"


/**
 * moves cursor to a certain direction
 * 
 * @param direction moving direction, 1 for right, -1 for left
 * 
 * @return success, if coordinate is blocked or is an invalid char then return 0, else return 1
 */
uint8_t move_cursor(int direction);

/**
 * Deletes a char on the left of the cursor
 * 
 * @attention this function only handles the graphics, the reader is handled by backspace() in commands
 * 
 * @return success, if coordinate is blocked or is an invalid char then return 0, else return 1
 */
uint8_t backspace();

/**
 * Compares a directory entry with an empty directory
 * 
 * @param in    DirectoryEntry to be compared
 * 
 * @return      If in is empty, returns 1. 0 otherwise
 */
uint8_t is_entry_empty(DirectoryEntry in);

/**
 * Reads current cluster as a folder
 * @warning                         this function uses malloc, be sure to close the reader afterwards
 * 
 * @param cluster_number            current folder cluster number
 * 
 * @return                          a struct containing the read data and number of read clusters
 */
FAT32DirectoryReader get_self_dir_info(uint32_t current_cluster);

//TODO: Document
void animation();

/**
 * Prints information of current cluster
 * 
 * @param clusterCluster            current folder cluster number
 */
void dir(uint32_t currentCluster);

/**
 * Clears screen and reader
 */
void clear();

/**
 * Creates new folder
 */
void mkdir(char *dirname, uint32_t currentCluster);

void whereis(uint16_t cluster_number, char* filename, FAT32DriverRequest* result_array, uint16_t* result_count);
#endif