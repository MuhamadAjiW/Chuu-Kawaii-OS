
#include "../lib-header/stdtype.h"

#define RESERVED_CLUSTER_NUMBER 0
#define FAT_CLUSTER_NUMBER 1
#define ROOT_CLUSTER_NUMBER 2

#define BLOCK_SIZE 4096
#define SECTOR_SIZE 512
#define CLUSTER_SIZE 64


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

struct FAT32FileAllocationTable{
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
    DirectoryEntry entry[CLUSTER_SIZE];
}__attribute__((packed));
typedef struct DirectoryTable DirectoryTable;

struct FAT32DriverState{
    uint8_t state;
}__attribute__((packed));
typedef struct FAT32DriverState FAT32DriverState;

struct ClusterBuffer{
    uint32_t buf[CLUSTER_SIZE];
}__attribute__((packed));
typedef struct ClusterBuffer ClusterBuffer;

struct FAT32DriverRequest{
    ClusterBuffer* buf;
    char name[8];
    char ext[3];
    uint8_t parent_cluster_number;
    uint32_t buffer_size;
}__attribute__((packed));
typedef struct FAT32DriverRequest FAT32DriverRequest;

void initialize_filesystem_fat32();
bool is_empty_storage();
void create_fat32();

void read(FAT32DriverRequest);
void read_directory();
void write(FAT32DriverRequest);
void del(FAT32DriverRequest);
void init_directory_table();
void cluster_to_lba();

