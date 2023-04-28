
#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "stdtype.h"
#include "stdio.h"

#include "user-shell.h"

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
 * Prints information of current cluster
 * 
 * @param clusterCluster            current folder cluster number
 */
void ls(uint32_t currentCluster);

/**
 * Move directory
 * 
 * @param pathname         Pathname to be checked
 * @param directory_info   Struct directory that it is on
 * 
 * @return      If in is empty, returns 1. 0 otherwise
 */
directory_info cd(char* pathname, directory_info directory_info);

void rm(uint32_t currentCluster);

void cat(uint32_t currentCluster);
void cp(uint32_t currentCluster);
void mv(uint32_t currentCluster);
void copy1Folder(FAT32DriverRequest src, FAT32DriverRequest dest);
void copy1File(FAT32DriverRequest src, FAT32DriverRequest dest);

/**
 * Creates new folder
 */
void mkdir(char *dirname, uint32_t currentCluster);

void whereis(uint16_t current_cluster, char* filename, char* path);
#endif