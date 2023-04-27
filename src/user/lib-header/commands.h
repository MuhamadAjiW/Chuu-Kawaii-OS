
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

void rm(int currentCluster);

void cat(int currentCluster);
void cp(int currentCluster);
#endif