#ifndef COMMANDS_UTIL
#define COMMANDS_UTIL

#include "stdtype.h"
#include "stdio.h"

#include "user-shell.h"

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
 * Check if the path is valid
 * 
 * @param pathname          Pathname to be checked
 * @param current_cluster   The cluster that it is on
 * 
 * @return      If in is empty, returns 1. 0 otherwise
 */
uint8_t is_directorypath_valid(char* pathname, uint32_t current_cluster);

/**
 * Check if the path is valid
 * 
 * @param pathname          Pathname to be checked
 * @param current_cluster   The cluster that it is on
 * 
 * @return      If in is empty, returns 1. 0 otherwise
 */
uint8_t is_filepath_valid(char* pathname, uint32_t current_cluster);

/**
 * Returns cluster from a path string
 * 
 * @param pathname          Pathname to be checked
 * @param current_cluster   The cluster that it is on
 * 
 * @return      If in is empty, returns 1. 0 otherwise
 */
uint32_t path_to_cluster(char* pathname, uint32_t current_cluster);


FAT32DriverRequest path_to_file_request();

FAT32DriverRequest path_to_dir_request();

/**
 * Reads current cluster as a folder
 * @warning                         this function uses malloc, be sure to close the reader afterwards
 * 
 * @param cluster_number            current folder cluster number
 * 
 * @return                          a struct containing the read data and number of read clusters
 */
FAT32DirectoryReader get_self_dir_info(uint32_t current_cluster);

#endif