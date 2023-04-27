
#ifndef _STDIO_H
#define _STDIO_H

#include "stdtype.h"

// Necessary filesystem related macros
#define END_OF_FILE 0xfffffff8

#define RESERVED_CLUSTER_NUMBER 0
#define FAT_CLUSTER_NUMBER 1
#define ROOT_CLUSTER_NUMBER 2

#define CLUSTER_SIZE 2048
#define SECTOR_COUNT 64

/**
 * Struct containing the value of bytes in a cluster
 * 
 * @param buf           Array of each byte
 */
struct ClusterBuffer{
    uint8_t buf[CLUSTER_SIZE];
}__attribute__((packed));
typedef struct ClusterBuffer ClusterBuffer;

/**
 * Struct containing the FAT table
 * 
 * @param sector_next   Array of next cluster location
 */
struct FAT32FileAllocationTable{
    uint32_t sector_next[CLUSTER_SIZE/4];
}__attribute__((packed));
typedef struct FAT32FileAllocationTable FAT32FileAllocationTable;

/**
 * Struct for FAT entries
 * Should be self explanatory
 * 
 */
struct DirectoryEntry{
    char filename[8];
    char extension[3];

    uint8_t read_only : 1 ;
    uint8_t hidden : 1 ;
    uint8_t system : 1 ;
    uint8_t volume_id : 1 ;
    uint8_t directory : 1 ;
    uint8_t archive : 1 ;
    uint8_t resbit1 : 1 ;
    uint8_t resbit2 : 1 ;
    
    uint8_t reserved;
    
    uint8_t creation_time_low;
    uint16_t creation_time_seconds : 5;
    uint16_t creation_time_minutes : 6;
    uint16_t creation_time_hours : 5;
    
    uint16_t creation_time_day : 5;
    uint16_t creation_time_month : 4;
    uint16_t creation_time_year : 7;

    uint16_t accessed_time_day : 5;
    uint16_t accessed_time_month : 4;
    uint16_t accessed_time_year : 7;

    uint16_t high_bits;

    uint16_t modification_time_seconds : 5;
    uint16_t modification_time_minutes : 6;
    uint16_t modification_time_hours : 5;

    uint16_t modification_time_day : 5;
    uint16_t modification_time_month : 4;
    uint16_t modifcation_time_year : 7;

    uint16_t cluster_number;
    uint32_t size;
}__attribute__((packed));
typedef struct DirectoryEntry DirectoryEntry;

/**
 * Struct for folders
 * Index 0 always contains parent info, even in extension tables
 * 
 * @param entry file FAT entry
 */
struct DirectoryTable{
    DirectoryEntry entry[CLUSTER_SIZE/sizeof(DirectoryEntry)];
}__attribute__((packed));
typedef struct DirectoryTable DirectoryTable;

/**
 * Struct for CRUD request
 * 
 * @param buf                   assigned buffer location for load, unused for other operations
 * @param name                  file name
 * @param ext                   file extension
 * @param parent_cluster_number parent cluster location
 * @param buffer_size           assigned buffer size for load, unused for other operations
 */
struct FAT32FileReader{
    uint32_t cluster_count;
    ClusterBuffer* content;
}__attribute__((packed));
typedef struct FAT32FileReader FAT32FileReader;

/**
 * Struct for reading files
 * 
 * @param cluster_count         number of clusters containing the read file
 * @param content               pointer to the actual content of the file divided to clusters
 */
struct FAT32DirectoryReader{
    uint32_t cluster_count;
    DirectoryTable* content;
}__attribute__((packed));
typedef struct FAT32DirectoryReader FAT32DirectoryReader;

/**
 * Struct for reading folders
 * 
 * @param cluster_count         number of clusters containing the read folder
 * @param content               pointer to the actual content of the folder divided to clusters
 */
struct FAT32DriverRequest{
    void* buf;
    char name[8];
    char ext[3];
    uint32_t parent_cluster_number;
    uint32_t buffer_size;
}__attribute__((packed));
typedef struct FAT32DriverRequest FAT32DriverRequest;

// Shell IO functions
/**
 * Assigns null terminated string with default colors at current cursor location to writable buffer
 * @warning         Do not use on non-null-terminated strings
 * 
 * @param string    written string
 */
void print(char* string);

/**
 * Writes null terminated string with user-defined colors at current cursor location
 * @warning         Do not use on non-null-terminated strings
 * 
 * @param string    written string
 * @param color     index of color palette
 */
void print_color(char* string, uint8_t color);

/**
 * Assigns char with default colors at current cursor location to writable buffer
 * 
 * @param c         written char
 */
void print_char(char c);

/**
 * Assigns char with default colors at current cursor location to writable buffer
 * 
 * @param c         written char
 * @param color     index of color palette
 */
void print_char_color(char c, uint8_t color);

/**
 * Gets keyboard last pressed key and clears it
 * 
 * @return          last pressed key
 */
char getc();


// File IO functions
/**
 * Read a file from a request
 * @warning         this function uses malloc, be sure to close the reader afterwards
 * 
 * @param request   requested file
 * 
 * @return          a struct containing the read data and number of read clusters
 */
FAT32FileReader readf(FAT32DriverRequest request);

/**
 * Read a folder from a request
 * @warning         this function uses malloc, be sure to close the reader afterwards
 * 
 * @param request   requested folder
 * 
 * @return          a struct containing the read data and number of read clusters
 */
FAT32DirectoryReader readf_dir(FAT32DriverRequest request);

/**
 * Unallocates file reader buffer
 * @warning         this function is for allocated pointers, do not use on unallocated pointers
 * 
 * @param pointer   an allocated file reader pointer
 * 
 */
void closef(FAT32FileReader request);

/**
 * Unallocates folder reader buffer
 * @warning         this function is for allocated pointers, do not use on unallocated pointers
 * 
 * @param pointer   an allocated folder reader pointer
 * 
 */
void closef_dir(FAT32DirectoryReader request);

/**
 * Writes a requested file to the filesystem
 * 
 * @param request   requested file
 * 
 * @return          0 means success, anything other than 0 are error codes
 */
uint8_t writef(FAT32DriverRequest request);

/**
 * Deletes a fat directory entry in a directory cluster
 * 
 * @param request    requested file
 * 
 * @return           Status of deletion, 0 is success, other than 0 are error codes
 */
uint8_t deletef(FAT32DriverRequest request);


/**
 * Read clusters from the disk
 * 
 * @param reader                    destination buffer to store the read data
 * @param cluster                   cluster index to read
 * @param sector_count              number of clusters to read
 */
void readcluster(void* reader, uint16_t cluster, uint16_t sector_count);

/**
 * Copies a reader buffer and returns it as a directory table
 *  Seems redundant but quite useful in a lot of cases. Might be a subject to refactor.
 * 
 * @param reader                    pointer to a reader buffer
 * 
 * @return                          reader as directory table, contents are copied and not as a casted pointer
 */
DirectoryTable asdirectory(uint32_t* reader);

/**
 * Checks whether a cluster is a directory, uses reserved bits and directory flags, still might not be 100% accurate
 * 
 * @param cluster                   Cluster to check
 * 
 * @return                          1 if is a folder, 0 if not
 */
uint8_t isdirectory(uint32_t cluster);

#endif