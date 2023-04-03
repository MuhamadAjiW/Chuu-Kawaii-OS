
#include "../lib-header/commands.h"

#include "../lib-header/stdio.h"
#include "../lib-header/string.h"
#include "../lib-header/stdlib.h"
#include "../lib-header/syscall.h"

DirectoryEntry emptyEntry = {0};

uint8_t is_empty_empty(DirectoryEntry in){
    return (in.filename == 0 &&
        in.extension == 0 &&
        in.read_only == 0 &&
        in.hidden == 0 &&
        in.system == 0 &&
        in.volume_id == 0 &&
        in.directory == 0 &&
        in.archive == 0 &&
        in.resbit1 == 0 &&
        in.resbit2 == 0 &&
        in.reserved == 0 &&
        in.creation_time_low == 0 &&
        in.creation_time_seconds == 0 &&
        in.creation_time_minutes == 0 &&
        in.creation_time_hours == 0 &&
        in.creation_time_day == 0 &&
        in.creation_time_month == 0 &&
        in.creation_time_year == 0 &&
        in.accessed_time_day == 0 &&
        in.accessed_time_month == 0 &&
        in.accessed_time_year == 0 &&
        in.high_bits == 0 &&
        in.modification_time_seconds == 0 &&
        in.modification_time_minutes == 0 &&
        in.modification_time_hours == 0 &&
        in.modification_time_day == 0 &&
        in.modification_time_month == 0 &&
        in.modifcation_time_year == 0 &&
        in.cluster_number == 0 &&
        in.size == 0
    );
}

/*
void dir(uint32_t currentCluster, char dirname[8]){
    struct FAT32DriverRequest request = {
        .parent_cluster_number = 2,
        .name = dirname,
    };

    FAT32DirectoryReader reader;
    reader = readf_dir(request);

    for(int i = 0; i < reader.cluster_count){
        for(int j = 1; j < SECTOR_COUNT; j++){
            if(is_empty_empty(reader.content->entry ))
        }
    }

    uint32_t reader[CLUSTER_SIZE/4] = {0};
    DirectoryTable table;
    read_clusters((void*)reader, currentCluster, 1);

    table = read_directory(reader);
    
    int counter = 1;
    char char_buffer[9];
    
    print("    Parent folder: ");

    for(int j = 0; j < 8; j++){
        graphics_write_char(table.entry[0].filename[j]);
    }
    print("\n    No   Name        Ext    Size      Creation time");

    uint32_t current_cluster = currentCluster;
    if(current_cluster != END_OF_FILE){
        while (current_cluster != END_OF_FILE){
            read_clusters((void*)reader, current_cluster, 1);
            table = read_directory(reader);
            
            for(int i = 1; i < 64; i++){
                if(memcmp(&table.entry[i], &emptyEntry, 32) != 0){

                    print("\n    ");
                    int_toString(counter, char_buffer);
                    print(char_buffer);
                    print(".");
                    for(int i = strlen(char_buffer); i < 4; i++){
                        print(" ");
                    }

                    for(int j = 0; j < 8; j++){
                        if(table.entry[i].filename[j] == 0){
                           print_char(' ');
                        }
                        print_char(table.entry[i].filename[j]);
                    }

                    print("    ");
                    for(int j = 0; j < 3; j++){
                        if(table.entry[i].extension[j] == 0){
                            print_char(' ');
                        }
                        else print_char(table.entry[i].extension[j]);
                    }
                    
                    print("    ");
                    int_toString(table.entry[i].size, char_buffer);
                    print(char_buffer);


                    for(int i = strlen(char_buffer); i < 10; i++){
                        print(" ");
                    }

                    int_toString(table.entry[i].creation_time_hours, char_buffer);
                    print(char_buffer);
                    print(":");
                    int_toString(table.entry[i].creation_time_minutes, char_buffer);
                    print(char_buffer);
                    print(":");
                    int_toString(table.entry[i].creation_time_seconds, char_buffer);
                    print(char_buffer);

                    print("  ");
                    int_toString(table.entry[i].creation_time_day, char_buffer);
                    print(char_buffer);
                    print("/");
                    int_toString(table.entry[i].creation_time_month, char_buffer);
                    print(char_buffer);
                    print("/");
                    int_toString((table.entry[i].creation_time_year) + get_cmos_data().century * 100, char_buffer);
                    print(char_buffer);

                    counter++;
                }
            }

            read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
            current_cluster = reader[current_cluster];
        }
    }           
}
*/

void clear(){
    syscall(SYSCALL_CLEAR_SCREEN, 0, 0, 0);
}

uint8_t move_cursor(int direction){
    uint8_t success = 0;
    syscall(SYSCALL_MOVE_CURSOR, (uint32_t) direction, (uint32_t) &success, 0);
    return success;
}

uint8_t backspace(){
    uint8_t success = 0;
    syscall(SYSCALL_BACKSPACE, (uint32_t) &success, 0, 0);
    return success;
}
