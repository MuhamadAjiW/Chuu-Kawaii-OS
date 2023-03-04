#include "../lib-header/fat32.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/disk.h"
#include "../lib-header/framebuffer.h"
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

static DirectoryEntry emptyEntry = {0};

FAT32FileAllocationTable fat;
DirectoryEntry* root_directory;

char buffer[128]; 

void initialize_filesystem_fat32(){
    uint32_t* entry = 0;
    uint32_t* entry2 = 0;

    for(int i = 0; i < 4096; i++){
        entry[i] = 0;
        entry2[i] = 0;
    }

    entry2[0] = 'f' | ('s' << 8) | ('_' << 16) | ('s' << 24);
    entry2[1] = 'i' | ('g' << 8) | ('n' << 16) | ('a' << 24);
    entry2[2] = 't' | ('u' << 8) | ('r' << 16) | ('e' << 24);
    write_blocks(cluster_to_lba(0), 1, entry2);
    init_directory_table(2);
    for (int i = 0; i < 3; i++){
        entry[i] = END_OF_FILE;
    }
    write_blocks(cluster_to_lba(1), 1, entry);

    return;
}

void create_fat32(FAT32DriverRequest request, uint16_t cluster_number){
    uint32_t* reader = 0;
    for(int i = 0; i < 4096; i++){
        reader[i] = 0;
    }

    DirectoryEntry add = {
        .filename = {*request.name},
        .extension = {*request.ext},
        .size = request.buffer_size
    };

    if (request.buffer_size == 0){
        add.directory = 1;
        init_directory_table(cluster_number);
    }
    
    read_blocks(reader, cluster_to_lba(request.parent_cluster_number), 1);
    
    int i;
    int limit = SECTOR_COUNT * 2;
    for(i = 0; i < limit; i += 2){
        if(reader[i] == 0 && reader[i+1] == 0){
            break;
        }
    }
    if(i < limit){
        memcpy((reader + i*32), &add, 4);
        write_blocks(cluster_to_lba(request.parent_cluster_number), 1, reader);
    }

    return;
}


void write(FAT32DriverRequest request){
    uint32_t* reader = 0;
    for(int i = 0; i < 4096; i++){
        reader[i] = 0;
    }

    read_blocks(reader, cluster_to_lba(1), 1);
    
    uint32_t size = request.buffer_size;
    int index = 0;
    int cachedindex = 0;
    
    bool large = 0;
    do{
        uint16_t i = 3;
        
        for (i = 3; i < SECTOR_COUNT; i++){
            if(reader[i] == 0){
                break;
            }
        }
        
        if(size != 0){
            write_blocks(cluster_to_lba(i), 1, (uint32_t*) &request.buf[index]);
            index++;
        }

        create_fat32(request, i);
        read_blocks(reader, cluster_to_lba(1), 1);

        if(large){
            reader[cachedindex] = i;
        }

        if(size < CLUSTER_SIZE){
            reader[i] = END_OF_FILE;
            size = 0;
        }
        else{
            size -= CLUSTER_SIZE;
            large = 1;
            cachedindex = i;
        }
    } while (size != 0);
    

    write_blocks(cluster_to_lba(1), 1, reader);
    return;
}


void init_directory_table(uint16_t cluster_number){
    DirectoryTable table;
    for(int i = 0; i < 64; i++){
        table.entry[i] = emptyEntry;
    }
    write_blocks(cluster_to_lba(cluster_number), 1, (uint32_t*) &table);

    return;
}

DirectoryTable read_directory(uint32_t* reader){
    DirectoryTable table;
    memcpy(&table, reader, CLUSTER_SIZE/8);

    return table;
}


/*
bool is_empty_storage(DirectoryTable table){}
void read(){}
void del(){}
*/

int cluster_to_lba(int clusters){
    return CLUSTER_SIZE * clusters;
}
