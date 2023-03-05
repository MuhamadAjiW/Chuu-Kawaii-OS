#include "../lib-header/fat32.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/disk.h"
#include "../lib-header/memory_manager.h"
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

DirectoryEntry emptyEntry = {0};
//static char buffer[CLUSTER_SIZE/4]; //debug purposes

FAT32FileAllocationTable fat;
DirectoryEntry* root_directory;

void initialize_filesystem_fat32(){
    uint32_t entry[CLUSTER_SIZE/4];
    uint32_t entry2[CLUSTER_SIZE/4];

    for(int i = 0; i < CLUSTER_SIZE/4; i++){
        entry[i] = 0;
        entry2[i] = 0;
    }

    entry2[0] = 'f' | ('s' << 8) | ('_' << 16) | ('s' << 24);
    entry2[1] = 'i' | ('g' << 8) | ('n' << 16) | ('a' << 24);
    entry2[2] = 't' | ('u' << 8) | ('r' << 16) | ('e' << 24);
    write_clusters((void*)entry2, 0, 1);
    init_directory_table(2, ROOT_CLUSTER_NUMBER);

    for (int i = 0; i < 3; i++){
        entry[i] = END_OF_FILE;
    }
    write_clusters((void*)entry, 1, 1);

    return;
}

void create_fat32(FAT32DriverRequest request, uint16_t cluster_number){
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    for(int i = 0; i < CLUSTER_SIZE/4; i++){
        reader[i] = 0;
    }

    DirectoryEntry add = {
        .filename = {0},
        .extension = {0},
        .read_only = 0,
        .hidden = 0,
        .system = 0,
        .volume_id = 0,
        .directory = 0,
        .archive = 0,
        .resbit1 = 0,
        .resbit2 = 0,
        .reserved = 0,
        .creation_time_low = 0,
        .creation_time_seconds = 0,
        .creation_time_minutes = 0,
        .creation_time_hours = 0,
        .creation_time_day = 0,
        .creation_time_month = 0,
        .creation_time_year = 0,
        .accessed_time_day = 0,
        .accessed_time_month = 0,
        .accessed_time_year = 0,
        .high_bits = 0,
        .modification_time_seconds = 0,
        .modification_time_minutes = 0,
        .modification_time_hours = 0,
        .modification_time_day = 0,
        .modification_time_month = 0,
        .modifcation_time_year = 0,
        .cluster_number = cluster_number,
        .size = request.buffer_size
    };

    if (request.buffer_size == 0){
        add.directory = 1;
        init_directory_table(cluster_number, request.parent_cluster_number);
    }
    memcpy(add.filename, request.name, 8);
    memcpy(add.extension, request.ext, 8);

    read_blocks(reader, cluster_to_lba(request.parent_cluster_number), 1);
    
    DirectoryTable table = read_directory(reader);
    int i;
    for(i = 0; i < SECTOR_COUNT; i ++){
        if(memcmp(&table.entry[i], &emptyEntry, 32) == 0){
            break;
        }
    }
    if(i < SECTOR_COUNT){
        memcpy((reader + i*8), &add, 32);
        void* writer = (void*) reader;
        write_clusters(writer, request.parent_cluster_number, 1);
    }
    return;
}


void write(FAT32DriverRequest request){
    uint32_t reader[CLUSTER_SIZE/4] = {0};

    read_blocks(reader, cluster_to_lba(1), 1);
    
    uint32_t size = request.buffer_size;
    int index = 0;
    int cachedindex = 0;
    
    bool large = 0;
    bool running = 1;
    bool created = 0;
    uint16_t i = 3;
    while (running){
        for (i = 3; i < SECTOR_COUNT; i++){
            if(reader[i] == 0 && i != cachedindex){
                break;
            }
        }

        if (!created){
            create_fat32(request, i);
            created = 1;
        }
        
        if(size != 0){
            void* writer = (void*) &request.buf[index];
            write_clusters(writer, i, 1);
            index++;
        }

        if(large){
            reader[cachedindex] = i;
        }

        if(size <= CLUSTER_SIZE){
            reader[i] = END_OF_FILE;
            running = 0;
        }
        else{
            size -= CLUSTER_SIZE;
            large = 1;
            cachedindex = i;
        }
    };
    
    
    void* writer = (void*) &reader;
    write_clusters(writer, 1, 1);
    return;
}


void init_directory_table(uint16_t cluster_number, uint16_t parent_cluster_number){
    DirectoryTable table;
    //TODO: add parent entry in index 0

    if (parent_cluster_number == ROOT_CLUSTER_NUMBER){
        DirectoryEntry parent = {
            .filename = {'r', 'o', 'o', 't',' ', ' ', ' ', ' '},
            .extension = {' ', ' ', ' '},
            .read_only = 0,
            .hidden = 0,
            .system = 0,
            .volume_id = 0,
            .directory = 1,
            .archive = 0,
            .resbit1 = 0,
            .resbit2 = 0,
            .reserved = 0,
            .creation_time_low = 0,
            .creation_time_seconds = 0,
            .creation_time_minutes = 0,
            .creation_time_hours = 0,
            .creation_time_day = 0,
            .creation_time_month = 0,
            .creation_time_year = 0,
            .accessed_time_day = 0,
            .accessed_time_month = 0,
            .accessed_time_year = 0,
            .high_bits = 0,
            .modification_time_seconds = 0,
            .modification_time_minutes = 0,
            .modification_time_hours = 0,
            .modification_time_day = 0,
            .modification_time_month = 0,
            .modifcation_time_year = 0,
            .cluster_number = ROOT_CLUSTER_NUMBER,
            .size = 0
        };
        table.entry[0] = parent;
    }
    else{
        DirectoryEntry parent = get_parent_info(parent_cluster_number);
        table.entry[0] = parent;
    }


    for(int i = 1; i < 64; i++){
        table.entry[i] = emptyEntry;
    }
    
    void* writer = (void*) &table;
    write_clusters(writer, cluster_number, 1);

    return;
}

void delete(FAT32DriverRequest request){
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    uint32_t empty_cluster[CLUSTER_SIZE/4] = {0};
    DirectoryEntry self = get_self_info(request);
    uint32_t marker = 0;
    uint16_t current_cluster = 0;
    uint8_t deleting = 0;
    void* writer;

    if(self.directory == 1){
        deleteFolder(self.cluster_number);
    }

    read_blocks(reader, cluster_to_lba(FAT_CLUSTER_NUMBER), 1);

    marker = reader[current_cluster];
    current_cluster = self.cluster_number;
    deleting = 1;
    
    while (deleting){
        reader[current_cluster] = 0;
        if(marker == END_OF_FILE){
            writer = (void*) empty_cluster;
            write_clusters(writer, current_cluster, 1);
            deleting = 0;
        }
        else{
            current_cluster = marker;
            marker = reader[current_cluster];
        }
    }
    writer = (void*) reader;
    write_clusters(writer, FAT_CLUSTER_NUMBER, 1);    

    DirectoryTable parent_table;
    for(int i = 0; i < CLUSTER_SIZE/4; i++){
        reader[i] = 0;
    }
    read_blocks(reader, cluster_to_lba(request.parent_cluster_number), 1);
    parent_table = read_directory(reader);

    for(int i = 0; i < 64; i++){
        if(memcmp(&parent_table.entry[i].filename, &request.name, 8) == 0 && memcmp(&parent_table.entry[i].extension, &request.ext, 3) == 0){
            parent_table.entry[i] = emptyEntry;
            break;
        }
    }
}

void deleteFolder(uint16_t cluster_number){
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    DirectoryTable table;
    FAT32DriverRequest request;
    read_blocks(reader, cluster_to_lba(cluster_number), 1);
    table = read_directory(reader);
    for(int i = 0; i < 64; i++){
        if (memcmp(&table.entry[i], &emptyEntry, 32) != 0){
            memcpy(&request.name, &table.entry[i].filename, 8);
            memcpy(&request.ext, &table.entry[i].extension, 3);
            request.parent_cluster_number = cluster_number;

            delete(request);
        }
    }
}

DirectoryEntry get_parent_info(uint16_t parent_cluster_number){
    DirectoryEntry info;
    DirectoryTable table;

    uint32_t reader[CLUSTER_SIZE/4] = {0};
    read_blocks(reader, cluster_to_lba(parent_cluster_number), 1);
    table = read_directory(reader);
    read_blocks(reader, cluster_to_lba(table.entry[0].cluster_number), 1);
    table = read_directory(reader);

    for(int i = 0; i < 64; i++){
        if(table.entry[i].cluster_number == parent_cluster_number){
            info = table.entry[i];
            break;
        }
    }

    return info;
}

DirectoryEntry get_self_info(FAT32DriverRequest request){
    DirectoryEntry info;
    DirectoryTable table;

    uint32_t reader[CLUSTER_SIZE/4] = {0};
    read_blocks(reader, cluster_to_lba(request.parent_cluster_number), 1);
    table = read_directory(reader);

    for(int i = 0; i < 64; i++){
        if(memcmp(&table.entry[i].filename, &request.name, 8) == 0 && memcmp(&table.entry[i].extension, &request.ext, 3) == 0){
            info = table.entry[i];
            break;
        }
    }

    return info;
}


DirectoryTable read_directory(uint32_t* reader){
    DirectoryTable table;
    memcpy(&table, reader, CLUSTER_SIZE);
    return table;
}


void read_clusters(ClusterBuffer* target, uint16_t cluster, uint16_t sector_count){
    uint32_t reader[128] = {0};
    read_blocks(reader, cluster_to_lba(cluster), sector_count);
    memcpy(target, &reader, 512);
    read_blocks(reader, cluster_to_lba(cluster) + 512, sector_count);
    memcpy(&target->buf[512], &reader, 512);
    read_blocks(reader, cluster_to_lba(cluster) + 1024, sector_count);
    memcpy(&target->buf[1024], &reader, 512);
    read_blocks(reader, cluster_to_lba(cluster) + 1536, sector_count);
    memcpy(&target->buf[1536], &reader, 512);
};


void write_clusters(ClusterBuffer* entry, uint16_t cluster, uint16_t sector_count){
    uint32_t writer[128] = {0};
    memcpy(writer, entry, 512);
    write_blocks(cluster_to_lba(cluster), sector_count, writer);
    memcpy(writer, &entry->buf[512], 512);
    write_blocks(cluster_to_lba(cluster) + 512, sector_count, writer);
    memcpy(writer, &entry->buf[1024], 512);
    write_blocks(cluster_to_lba(cluster) + 1024, sector_count, writer);
    memcpy(writer, &entry->buf[1536], 512);
    write_blocks(cluster_to_lba(cluster) + 1536, sector_count, writer);
};
/*
bool is_empty_storage(DirectoryTable table){
    for(int i = 1; i < SECTOR_COUNT){
        if(memcmp(&table.entry[i], &emptyEntry, 32) != 0){
            return false;
        }
    }
    return true;
}
*/

ClusterBuffer* read(FAT32DriverRequest request){
    ClusterBuffer* output = 0;
    DirectoryEntry self = get_self_info(request);

    if(request.buffer_size < self.size){
        return 0;
    }
    else{
        bool reading = 1;
        uint16_t index = 0;

        uint32_t reader[CLUSTER_SIZE/4] = {0};
        read_blocks(reader, cluster_to_lba(FAT_CLUSTER_NUMBER), 1);

        uint16_t current_cluster = self.cluster_number;
        uint32_t marker = reader[current_cluster];
        output = (ClusterBuffer*) malloc (((CLUSTER_SIZE + self.size - 1 )/CLUSTER_SIZE)*sizeof(ClusterBuffer));
        while (reading){
            read_clusters(output+index, current_cluster, 1); 

            if(marker == END_OF_FILE){
                reading = 0;
            }
            else{
                current_cluster = marker;
                marker = reader[current_cluster];
                index++;
            }
        }
    }

    return output;
}

void close(ClusterBuffer* pointer){
    free(pointer);
}

int cluster_to_lba(int clusters){
    return CLUSTER_SIZE * clusters;
}