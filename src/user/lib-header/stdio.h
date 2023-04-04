
#ifndef _STDIO_H
#define _STDIO_H

#include "stdtype.h"


#define END_OF_FILE 0xfffffff8

#define RESERVED_CLUSTER_NUMBER 0
#define FAT_CLUSTER_NUMBER 1
#define ROOT_CLUSTER_NUMBER 2

#define CLUSTER_SIZE 2048
#define SECTOR_COUNT 64


struct ClusterBuffer{
    uint8_t buf[CLUSTER_SIZE];
}__attribute__((packed));
typedef struct ClusterBuffer ClusterBuffer;

struct FAT32FileAllocationTable{
    uint32_t sector_next[CLUSTER_SIZE/4];
}__attribute__((packed));

typedef struct FAT32FileAllocationTable FAT32FileAllocationTable;

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

struct DirectoryTable{
    DirectoryEntry entry[CLUSTER_SIZE/sizeof(DirectoryEntry)];
}__attribute__((packed));
typedef struct DirectoryTable DirectoryTable;

struct FAT32FileReader{
    uint32_t cluster_count;
    ClusterBuffer* content;
}__attribute__((packed));
typedef struct FAT32FileReader FAT32FileReader;

struct FAT32DirectoryReader{
    uint32_t cluster_count;
    DirectoryTable* content;
}__attribute__((packed));
typedef struct FAT32DirectoryReader FAT32DirectoryReader;

struct FAT32DriverRequest{
    void* buf;
    char name[8];
    char ext[3];
    uint32_t parent_cluster_number;
    uint32_t buffer_size;
}__attribute__((packed));
typedef struct FAT32DriverRequest FAT32DriverRequest;

void print(char* string);
void print_char(char c);
char getc();

FAT32FileReader readf(FAT32DriverRequest request);
FAT32DirectoryReader readf_dir(FAT32DriverRequest request);
void closef(FAT32FileReader request);
void closef_dir(FAT32DirectoryReader request);
uint8_t writef(FAT32DriverRequest request);
uint8_t deletef(FAT32DriverRequest request);

#endif