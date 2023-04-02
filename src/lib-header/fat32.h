
#ifndef _FAT32_H
#define _FAT32_H

#include "stdtype.h"

#define END_OF_FILE 0xfffffff8

#define RESERVED_CLUSTER_NUMBER 0
#define FAT_CLUSTER_NUMBER 1
#define ROOT_CLUSTER_NUMBER 2

#define CLUSTER_SIZE 2048
#define SECTOR_COUNT 64

/*
#define BYTE_PER_SECTOR 4096
#define SECTORS_PER_CLUSTER 64
#define RESERVED_SECTORS 32
#define FAT_COUNT 2
#define DIRECTORY_ENTRY_COUNT 0
#define TOTAL_SECTORS 0
#define MEDIA_DESCRIPTOR_TYPE 0xf8
#define SECTORS_PER_FAT 0
#define SECTORS_PER_TRACK 0
#define HEADS 0
#define HIDDEN_SECTORS 0
#define LARGE_SECTOR_COUNT 1

#define SECTORS_PER_FAT_32 64
#define EXTERNAL_FLAGS 0
#define VERSION 0
#define ROOT_CLUSTER 2
#define FS_INFO 1
#define BK_BOOT_SECTOR 6
#define RESERVED 0
#define DRIVE_NUMBER 0x80
#define RESERVED1 0
#define BOOT_SIGNATURE 1
*/

/*
struct FAT32BootSector{
    uint8_t boot_jump_instrution[3];
    uint8_t oem_identifier[8];
    uint16_t bytes_per_Sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t directory_entry_count;
    uint16_t total_sectors;
    uint8_t media_descriptor_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t large_sector_count;

    uint32_t sectors_per_fat_32;
    uint16_t external_flags;
    uint16_t file_system_version;
    uint32_t root_cluster;
    uint16_t file_system_info;
    uint16_t bk_boot_sector;
    uint8_t reserved[12];
    uint8_t drive_number;
    uint8_t reserved1;
    uint8_t boot_signature;
    uint32_t volume_id;
    uint8_t volume_label[11];
    uint8_t file_system_type[8];
}__attribute__((packed));
typedef struct FAT32BootSector FAT32BootSector;
*/
/*
static FAT32BootSector bootSector ={
    .boot_jump_instrution = {
        0xeb, 0x00, 0x90
    },
    .oem_identifier = {
        'M', 'S', 'W', 'I', 'N', '4', '.', '1'
    },
    .bytes_per_Sector = BYTE_PER_SECTOR,
    .sectors_per_cluster = SECTORS_PER_CLUSTER,
    .reserved_sectors = RESERVED_SECTORS,
    .fat_count = FAT_COUNT,
    .directory_entry_count = DIRECTORY_ENTRY_COUNT,
    .total_sectors = TOTAL_SECTORS,
    .media_descriptor_type = MEDIA_DESCRIPTOR_TYPE,
    .sectors_per_fat = SECTORS_PER_FAT,
    .sectors_per_track = SECTORS_PER_TRACK,
    .heads = HEADS,
    .hidden_sectors = HIDDEN_SECTORS,
    .large_sector_count = LARGE_SECTOR_COUNT,
    
    .sectors_per_fat_32 = SECTORS_PER_FAT_32,
    .external_flags = EXTERNAL_FLAGS,
    .file_system_version = VERSION,
    .root_cluster = ROOT_CLUSTER,
    .file_system_info = FS_INFO,
    .bk_boot_sector = BK_BOOT_SECTOR,
    .reserved = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    .drive_number = DRIVE_NUMBER,
    .reserved1 = RESERVED1,
    .boot_signature = BOOT_SIGNATURE,
    .volume_id = 0,
    .volume_label = {
        'N', 'O', ' ', 'N', 'A', 'M', 'E', ' ', ' ', ' ', ' '
    },
    .file_system_type = {
        'F', 'A', 'T', '3', '2', ' ', ' ', ' '
    }
};
*/

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

struct FAT32DriverState{
    uint8_t state;
}__attribute__((packed));
typedef struct FAT32DriverState FAT32DriverState;

struct FAT32DriverRequest{
    void* buf;
    char name[8];
    char ext[3];
    uint32_t parent_cluster_number;
    uint32_t buffer_size;
}__attribute__((packed));
typedef struct FAT32DriverRequest FAT32DriverRequest;

void initialize_filesystem_fat32();
void create_fat32(FAT32DriverRequest request, uint16_t cluster_number);
void init_directory_table(uint16_t cluster_number, uint16_t parent_cluster_number);
DirectoryEntry get_parent_info(uint16_t parent_cluster_number);
DirectoryEntry get_self_info(FAT32DriverRequest request);
DirectoryTable read_directory(uint32_t* reader);
int cluster_to_lba(int clusters);
uint8_t delete(FAT32DriverRequest);
void deleteFolder(uint16_t cluster_number);
uint8_t is_empty_storage();
void read_clusters(void*, uint16_t cluster, uint16_t sector_count);
void write_clusters(void*, uint16_t cluster, uint16_t sector_count);
void* read(FAT32DriverRequest request);
uint8_t load(FAT32DriverRequest request);
uint8_t write(FAT32DriverRequest request);
void close(ClusterBuffer* pointer);

uint32_t expand_folder(int cluster_number);
void update_file_time(DirectoryEntry *entry);
void update_file_size(DirectoryEntry* entry, uint32_t size, char category);

uint8_t is_directory(uint32_t cluster);
uint8_t name_exists(FAT32DriverRequest request);

#endif