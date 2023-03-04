#include "../lib-header/fat32.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/disk.h"
/*
static FAT32FileAllocationTable bootSector ={
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

DirectoryEntry* root_directory;

void initialize_filesystem_fat32(){
    
}



bool is_empty_storage(DirectoryTable table){
    uint8_t branches = 0;
    for(int i = 1; i < CLUSTER_SIZE; i++){
        if(table.entry == 0){
            break;
        }
        else{
            branches++;
        }
    }

    if (branches == 1) return 1;
    else return 0;
}
void create_fat32(){}

void FAT32DriverRequest(){}
void read(){}
void read_directory(){}
void write(){}
void del(){}
void init_directory_table(){}
void cluster_to_lba(){}
*/