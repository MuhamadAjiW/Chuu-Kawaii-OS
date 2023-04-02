//TODO: optimization, jorok banget ini
//TODO: comment juga, jelasin lah anjir cok ini codenya ngapain aja

#include "../lib-header/fat32.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/disk.h"
#include "../lib-header/cmos.h"
#include "../lib-header/memory_manager.h"
#include "../lib-header/graphics.h"
#include "../lib-header/string.h"

static cmos_reader cmos = {0};
DirectoryEntry emptyEntry = {0};

/* //debug purposes
static char buffer[CLUSTER_SIZE/4];
graphics_clear_buffer();
uint32_t reader2[CLUSTER_SIZE/4] = {0};
read_clusters((void*)reader2, request.parent_cluster_number, 1);
for(int i = 0; i < 128; i++){
    int_toString((reader2[i]) >> 24, buffer);
    graphics_print(buffer);
    graphics_print(" ");
    int_toString(((reader2[i]) >> 16) & 0xff, buffer);
    graphics_print(buffer);
    graphics_print(" ");
    int_toString(((reader2[i]) >> 8) & 0xff, buffer);
    graphics_print(buffer);
    graphics_print(" ");
    int_toString((reader2[i]) & 0xff, buffer);
    graphics_print(buffer);
    graphics_print(" ");
}
*/

FAT32FileAllocationTable fat;
DirectoryEntry* root_directory;


uint8_t is_empty_storage(){
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    read_clusters((void*)reader, RESERVED_CLUSTER_NUMBER, 1);
    if(memcmp(reader, "fs_signature", 12) == 0){
        return 0;
    }
    return 1;
}


void initialize_filesystem_fat32(){
    if(is_empty_storage()){
        uint32_t entry[CLUSTER_SIZE/4] = {0};
        uint32_t entry2[CLUSTER_SIZE/4] = {0};

        entry2[0] = 'f' | ('s' << 8) | ('_' << 16) | ('s' << 24);
        entry2[1] = 'i' | ('g' << 8) | ('n' << 16) | ('a' << 24);
        entry2[2] = 't' | ('u' << 8) | ('r' << 16) | ('e' << 24);
        write_clusters((void*)entry2, 0, 1);
        DirectoryTable table = {
            .entry = 0
        };
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
            .size = 32
        };
        table.entry[0] = parent;

        void* writer = (void*) &table;
        write_clusters(writer, 2, 1);

        for (int i = 0; i < 3; i++){
            entry[i] = END_OF_FILE;
        }
        write_clusters((void*)entry, 1, 1);
    }

    return;
}

void create_fat32(FAT32DriverRequest request, uint16_t cluster_number){
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    cmos = get_cmos_data();
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
        .creation_time_seconds = cmos.second,
        .creation_time_minutes = cmos.minute,
        .creation_time_hours = cmos.hour,
        .creation_time_day = cmos.day,
        .creation_time_month = cmos.month,
        .creation_time_year = cmos.year,
        .accessed_time_day = cmos.day,
        .accessed_time_month = cmos.month,
        .accessed_time_year = cmos.year,
        .high_bits = 0,
        .modification_time_seconds = cmos.second,
        .modification_time_minutes = cmos.minute,
        .modification_time_hours = cmos.hour,
        .modification_time_day = cmos.day,
        .modification_time_month = cmos.month,
        .modifcation_time_year = cmos.year,
        .cluster_number = cluster_number,
        .size = request.buffer_size
    };
    if(request.buffer_size == 0){
        add.size = 32;
        add.directory = 1;
        init_directory_table(cluster_number, request.parent_cluster_number);
    }
    else{
        memcpy(add.extension, request.ext, 3);
    }

    memcpy(add.filename, request.name, 8);

    read_clusters((void*)reader, request.parent_cluster_number, 1);
    DirectoryTable table = read_directory(reader);
    int i;

    uint32_t current_cluster = request.parent_cluster_number;
    uint8_t added = 0;
    while(!added){
        for(i = 0; i < SECTOR_COUNT; i ++){
            if(memcmp(&table.entry[i], &emptyEntry, 32) == 0){
                added = 1;
                break;
            }
        }
        if(i < SECTOR_COUNT){
            memcpy(&table.entry[i], &add, 32);
            void* writer = (void*) &table;
            write_clusters(writer, current_cluster, 1);
        }
        else{
            read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
            current_cluster = reader[current_cluster];
            if(current_cluster == END_OF_FILE){
                current_cluster = expand_folder(request.parent_cluster_number);
                read_clusters((void*)reader, current_cluster, 1);
                table = read_directory(reader);
            }
            else{    
                read_clusters((void*)reader, current_cluster, 1);
                table = read_directory(reader);
            }
        }
    }

    return;
}


void update_size_recurse(FAT32DriverRequest request, uint16_t self_cluster, char category){
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    void* writer;
    
    //cek kalo root, endprocess, pasti bukan rekursi pertama
    if(request.parent_cluster_number == 2){
        DirectoryTable table = {0}; 
        uint32_t current_cluster = ROOT_CLUSTER_NUMBER;
        do
        {
            read_clusters((void*)reader, current_cluster, 1);                     //nambah size root
            table = read_directory(reader); 
            update_file_time(&table.entry[0]);
            update_file_size(&table.entry[0], request.buffer_size, category);
            for(int i = 1; i < SECTOR_COUNT; i++){
                if (table.entry[i].cluster_number == self_cluster){                         //nambah size subfolder
                    update_file_time(&table.entry[i]);
                    update_file_size(&table.entry[i], request.buffer_size, category);
                }
                else if ((memcmp(&table.entry[i], &emptyEntry, 32) != 0)){                                               //sinkronisasi size semua subfolder
                    read_clusters((void*)reader, table.entry[i].cluster_number, 1);
                    DirectoryTable table2 = read_directory(reader);
                    update_file_size(&table2.entry[0], request.buffer_size, category);
                    writer = (void*) &table2;
                    write_clusters(writer, table.entry[i].cluster_number, 1);
                }
            }
            writer = (void*) &table;
            write_clusters(writer, current_cluster, 1);

            read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
            current_cluster = reader[current_cluster];
        } while (current_cluster != END_OF_FILE);
    }
    //kalo bukan root, rekursi pertama
    else{
        DirectoryTable table = {0};
        uint32_t current_cluster = request.parent_cluster_number;
        
        if(request.buffer_size == 0){
            table.entry[0].size += 32;
        }
        else{
            table.entry[0].size += request.buffer_size;
        }

        if (self_cluster != 0){                                               //bukan rekursi pertama karena rekursi pertama harusnya belum merujuk folder
            do
            {
                read_clusters((void*)reader, current_cluster, 1);
                table = read_directory(reader); 
                update_file_time(&table.entry[0]);
                for(int i = 1; i < SECTOR_COUNT; i++){
                    if (table.entry[i].cluster_number == self_cluster){                         //nambah size subfolder
                        update_file_time(&table.entry[i]);
                        update_file_size(&table.entry[i], request.buffer_size, category);
                    }
                    else if ((memcmp(&table.entry[i], &emptyEntry, 32) != 0)){                                               //sinkronisasi size semua subfolder
                        read_clusters((void*)reader, table.entry[i].cluster_number, 1);
                        DirectoryTable table2 = read_directory(reader);
                        update_file_size(&table2.entry[0], request.buffer_size, category);
                        writer = (void*) &table2;
                        write_clusters(writer, table.entry[i].cluster_number, 1);
                    }
                }
                writer = (void*) &table;
                write_clusters(writer, current_cluster, 1);

                read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
                current_cluster = reader[current_cluster];
            } while (current_cluster != END_OF_FILE);
        }
        else{
            do
            {
                read_clusters((void*)reader, current_cluster, 1); 
                table = read_directory(reader); 
                for(int i = 1; i < SECTOR_COUNT; i++){
                    if (memcmp(&table.entry[i], &emptyEntry, 32) != 0){                                               //sinkronisasi size semua subfolder
                        read_clusters((void*)reader, table.entry[i].cluster_number, 1);
                        DirectoryTable table2 = read_directory(reader);
                        update_file_size(&table2.entry[0], request.buffer_size, category);
                        writer = (void*) &table2;
                        write_clusters(writer, table.entry[i].cluster_number, 1);
                    }
                }
                writer = (void*) &table;
                write_clusters(writer, current_cluster, 1);

                read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
                current_cluster = reader[current_cluster];
            } while (current_cluster != END_OF_FILE);
        }

        writer = (void*) &table;
        write_clusters(writer, request.parent_cluster_number, 1);

        FAT32DriverRequest next_request = {
            .buffer_size = request.buffer_size,
            .parent_cluster_number = table.entry[0].cluster_number
        };

        update_size_recurse(next_request, request.parent_cluster_number, category);
    }
}

void update_size(FAT32DriverRequest request, char category){
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    
    if(request.parent_cluster_number == 2){
        uint32_t current_cluster = ROOT_CLUSTER_NUMBER;
        DirectoryTable table = {0};
        cmos = get_cmos_data();
        void* writer;

        do
        {
            read_clusters((void*)reader, current_cluster, 1);                     //nambah size root
            table = read_directory(reader); 
            update_file_time(&table.entry[0]);
            update_file_size(&table.entry[0], request.buffer_size, category);
            for(int i = 0; i < SECTOR_COUNT; i++){
                if ((memcmp(&table.entry[i], &emptyEntry, 32) != 0)){                                               //sinkronisasi size semua subfolder
                    read_clusters((void*)reader, table.entry[i].cluster_number, 1);
                    DirectoryTable table2 = read_directory(reader);
                    update_file_size(&table2.entry[0], request.buffer_size, category);
                    writer = (void*) &table2;
                    write_clusters(writer, table.entry[i].cluster_number, 1);
                }
            }
            writer = (void*) &table;
            write_clusters(writer, current_cluster, 1);

            read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
            current_cluster = reader[current_cluster];
        } while (current_cluster != END_OF_FILE);
    }
    else{
        cmos = get_cmos_data();
        update_size_recurse(request, 0, category);
    }
}

uint8_t write(FAT32DriverRequest request){
    if(request.parent_cluster_number < 2){
        return 2;
    }
    else if (!is_directory(request.parent_cluster_number)){
        return 2;
    }
    else if (name_exists(request)){
        return 1;
    }

    update_size(request, '+');

    uint32_t reader[CLUSTER_SIZE/4] = {0};
    read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);

    uint32_t size = request.buffer_size;
    int index = 0;
    int cachedindex = 0;
    
    bool large = 0;
    bool running = 1;
    bool created = 0;
    uint16_t i = 3;

    while (running){
        for (i = 3; i < CLUSTER_SIZE/4; i++){ //ini untuk nulis entry di fat, jadi gak perlu handle untuk extendable folder
            if(reader[i] == 0 && i != cachedindex){
                break;
            }
        }

        if (!created){
            create_fat32(request, i);
            read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
            created = 1;
        }
        
        if(size != 0){
            void* writer = request.buf + index;
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
    return 0;
}


void init_directory_table(uint16_t cluster_number, uint16_t parent_cluster_number){
    DirectoryTable table = {.entry = 0};

    if (parent_cluster_number == ROOT_CLUSTER_NUMBER){
        uint32_t reader[CLUSTER_SIZE/4] = {0};
        read_clusters((void*)reader, ROOT_CLUSTER_NUMBER, 1);
        DirectoryTable root_table = read_directory(reader);
        table.entry[0] = root_table.entry[0];
    }
    else{
        DirectoryEntry parent = get_parent_info(parent_cluster_number);
        table.entry[0] = parent;
    }
    
    void* writer = (void*) &table;
    write_clusters(writer, cluster_number, 1);


    return;
}

uint8_t delete(FAT32DriverRequest request){
    update_size(request, '-');

    uint32_t reader[CLUSTER_SIZE/4] = {0};
    uint32_t empty_cluster[CLUSTER_SIZE/4] = {0};
    DirectoryEntry self = get_self_info(request);
    uint32_t marker = 0;
    uint32_t current_cluster = 0;
    uint8_t deleting = 0;
    void* writer;

    if(self.directory == 1){
        deleteFolder(self.cluster_number);
    }

    read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);

    current_cluster = self.cluster_number;
    marker = reader[current_cluster];
    deleting = 1;
    
    while (deleting){
        reader[current_cluster] = 0;
        writer = (void*) empty_cluster;
        write_clusters(writer, current_cluster, 1);
        if(marker == END_OF_FILE){
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
    uint32_t roaming_cluster = request.parent_cluster_number;
    uint8_t found = 0;

    while (!found && roaming_cluster != END_OF_FILE){
        read_clusters((void*)reader, roaming_cluster, 1);
        parent_table = read_directory(reader);
        for(int i = 1; i < SECTOR_COUNT; i++){
            if(memcmp(&parent_table.entry[i].filename, &request.name, 8) == 0){
                if (request.buffer_size == 0 ||
                    (request.buffer_size != 0 && memcmp(&parent_table.entry[i].extension, &request.ext, 3) == 0)){
                    parent_table.entry[i] = emptyEntry;
                    found = 1;
                    break;
                }
            }
        }
        if (!found){
            read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
            roaming_cluster = reader[roaming_cluster];
        }
    }
    
    if(roaming_cluster == END_OF_FILE){
        return 0;
    }
    
    writer = (void*) &parent_table;
    write_clusters(writer, roaming_cluster, 1);    

    return 1;
}

void deleteFolder(uint16_t cluster_number){
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    DirectoryTable table;
    FAT32DriverRequest request;

    uint32_t current_cluster = cluster_number;
    do{
        read_clusters((void*)reader, current_cluster, 1);
        table = read_directory(reader);
        for(int i = 1; i < SECTOR_COUNT; i++){
            if (memcmp(&table.entry[i], &emptyEntry, 32) != 0){
                memcpy(&request.name, &table.entry[i].filename, 8);
                memcpy(&request.ext, &table.entry[i].extension, 3);
                request.parent_cluster_number = cluster_number;

                delete(request);
            }
        }
        read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
        current_cluster = reader[current_cluster];

    } while (current_cluster != END_OF_FILE);

    return;
}

DirectoryEntry get_parent_info(uint16_t parent_cluster_number){
    DirectoryEntry info;
    DirectoryTable table;

    uint32_t reader[CLUSTER_SIZE/4] = {0};
    read_clusters((void*)reader, parent_cluster_number, 1);
    table = read_directory(reader);
    read_clusters((void*)reader, table.entry[0].cluster_number, 1);
    table = read_directory(reader);

    uint32_t current_cluster = parent_cluster_number;
    bool found = 0;
    while (!found)
    {
        for(int i = 0; i < SECTOR_COUNT; i++){
            if(table.entry[i].cluster_number == parent_cluster_number){
                info = table.entry[i];
                break;
            }
        }
        if (!found){
            //untuk extendable folder
            read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
            current_cluster = reader[current_cluster];
            read_clusters((void*)reader, current_cluster, 1);
            table = read_directory(reader);
        }
    }



    return info;
}

DirectoryEntry get_self_info(FAT32DriverRequest request){
    DirectoryEntry info;
    DirectoryTable table;

    uint32_t reader[CLUSTER_SIZE/4] = {0};
    uint32_t current_cluster = request.parent_cluster_number;
    read_clusters((void*)reader, current_cluster, 1);
    table = read_directory(reader);

    bool found = 0;
    uint8_t i;
    while (!found && current_cluster != END_OF_FILE)
    {
        for(i = 0; i < SECTOR_COUNT; i++){
            if(memcmp(&table.entry[i].filename, &request.name, 8) == 0){
                if (request.buffer_size == 0 ||
                    (request.buffer_size != 0 && memcmp(&table.entry[i].extension, &request.ext, 3) == 0)){

                    info = table.entry[i];
                    found = 1;
                }
            }
        }
        if (!found){
            //untuk extendable folder
            read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
            current_cluster = reader[current_cluster];
            read_clusters((void*)reader, current_cluster, 1);
            table = read_directory(reader);
        }
    }
    if (current_cluster == END_OF_FILE && !found)
        return emptyEntry;

    return info;
}


DirectoryTable read_directory(uint32_t* reader){
    DirectoryTable table;
    memcpy(&table, reader, CLUSTER_SIZE);
    return table;
}


void read_clusters(void* target, uint16_t cluster, uint16_t sector_count){
    read_blocks(target, cluster_to_lba(cluster), sector_count);
    read_blocks(target + 512, cluster_to_lba(cluster) + 1, sector_count);
    read_blocks(target + 1024, cluster_to_lba(cluster) + 2, sector_count);
    read_blocks(target + 1536, cluster_to_lba(cluster) + 3, sector_count);
};


void write_clusters(void* entry, uint16_t cluster, uint16_t sector_count){
    write_blocks(entry, cluster_to_lba(cluster), sector_count);
    write_blocks(entry + 512, cluster_to_lba(cluster) + 1, sector_count);
    write_blocks(entry + 1024, cluster_to_lba(cluster) + 2, sector_count);
    write_blocks(entry + 1536, cluster_to_lba(cluster) + 3, sector_count);
};

void* read(FAT32DriverRequest request){
    ClusterBuffer* output = 0;
    DirectoryEntry self = get_self_info(request);

    if(request.buffer_size < self.size){
        return 0;
    }
    else{
        bool reading = 1;
        uint16_t index = 0;

        uint32_t reader[CLUSTER_SIZE/4] = {0};
        read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);

        uint32_t current_cluster = self.cluster_number;
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

uint8_t load(FAT32DriverRequest request){
    void* output = (void*) request.buf;
    DirectoryEntry self = get_self_info(request);
    if (memcmp(&self, &emptyEntry, sizeof(DirectoryEntry)) == 0){
        return 0;
    }
    if(request.buffer_size < self.size){
        return 0;
    }
    else{
        bool reading = 1;
        uint16_t index = 0;

        uint32_t reader[CLUSTER_SIZE/4] = {0};
        read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);

        uint32_t current_cluster = self.cluster_number;
        uint32_t marker = reader[current_cluster];
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

    return 1;
}

void close(ClusterBuffer* pointer){
    free(pointer);
}

int cluster_to_lba(int clusters){
    return 4 * clusters;
}

uint32_t expand_folder(int cluster_number){
    void* writer;
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);

    //nyari yang kosong di fat
    uint32_t i = 0;
    uint8_t counter = 0;
    for (i = 3; i < CLUSTER_SIZE/4; i++){
        if(reader[i] == 0){
            counter++;
            if(counter > 1){
                break;
            }
        }
    }
    
    //nyari ujung cluster
    uint32_t traverse2 = cluster_number;
    uint32_t traverse = cluster_number;
    while (traverse != END_OF_FILE){
        traverse2 = traverse;
        traverse = reader[traverse];
    }
    //ekspansi cluster
    reader[traverse2] = i;
    reader[i] = END_OF_FILE;

    writer = (void*) &reader;
    write_clusters(writer, FAT_CLUSTER_NUMBER, 1);
    
    //inisiasi
    read_clusters((void*)reader, cluster_number, 1);

    DirectoryTable table = {0};
    table.entry[0] = read_directory(reader).entry[0];

    writer = (void*) &table;
    write_clusters(writer, i, 1);

    return i;
}

void update_file_time(DirectoryEntry* entry){
    //cmosnya gak dipanggil di sini siapa tau perlu buat waktu lain, inget aja buat manggil cmosnya
    entry->modification_time_seconds = cmos.second;
    entry->modification_time_minutes = cmos.minute;
    entry->modification_time_hours = cmos.hour;
    entry->modification_time_day = cmos.day;
    entry->modification_time_month = cmos.month;
    entry->modifcation_time_year = cmos.year;
}

void update_file_size(DirectoryEntry* entry, uint32_t size, char category){
    if(size == 0){
        if(category == '+'){
            entry->size += 32;

        }
        else if (category == '-'){
            entry->size -= 32;
        }
    }
    else{
        if(category == '+'){
            entry->size += size;

        }
        else if(category == '-'){
            entry->size -= size;

        }
    }
}

uint8_t is_directory(uint32_t cluster){
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    read_clusters((void*)reader, cluster, 1);
    DirectoryTable table = read_directory(reader);

    for(int i = 0; i < 10; i++){
        if(table.entry[i].reserved != 0){
            return 0;
        }
    }

    return 1;
}

uint8_t name_exists(FAT32DriverRequest request){
    uint32_t current_cluster = request.parent_cluster_number;
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    DirectoryTable table = {0};

    do{
        read_clusters((void*)reader, current_cluster, 1);
        table = read_directory(reader);
        for(int i = 1; i < SECTOR_COUNT; i++){
            if((memcmp(&table.entry[i].filename, request.name, 8) == 0) && 
                (memcmp(&table.entry[i].extension, request.ext, 3) == 0)
            ){
                return 1;
            }
        }

        read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
        current_cluster = reader[current_cluster];

     } while (current_cluster != END_OF_FILE);

    return 0;
}