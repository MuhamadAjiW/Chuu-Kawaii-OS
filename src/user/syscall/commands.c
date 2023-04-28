
#include "../lib-header/commands.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/stdio.h"
#include "../lib-header/string.h"
#include "../lib-header/time.h"
#include "../lib-header/stdlib.h"
#include "../lib-header/syscall.h"

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


uint8_t is_entry_empty(DirectoryEntry in){
    char* checker = (char*) &in;
    for(uint32_t i = 0; i < sizeof(DirectoryEntry); i++){
        if(checker[i] != 0) return 0;
    }
    return 1;
}

FAT32DirectoryReader get_self_dir_info(uint32_t current_cluster){
    FAT32DirectoryReader retval;
    syscall(SYSCALL_SELF_DIR_INFO, current_cluster, (uint32_t) &retval, 0);
    return retval;
}


void delay(uint32_t ms){
    uint32_t currentTick = 0;
    uint32_t cachedTick = 0;
    syscall(SYSCALL_GETTICK, (uint32_t) &currentTick, 0, 0);
    cachedTick = currentTick + ms;

    while (currentTick < cachedTick){
        syscall(SYSCALL_GETTICK, (uint32_t) &currentTick, 0, 0);
    }
}

#define ANIM_FRAMECOUNT 29
void animation(){
    for (uint8_t i = 0; i < ANIM_FRAMECOUNT; i++)
    {
        syscall(SYSCALL_ANIMATION, i, 0, 0);
        delay(100);
    }
}

void dir(uint32_t currentCluster){
    FAT32DirectoryReader directory_reader;

    directory_reader = get_self_dir_info(currentCluster);
    char char_buffer[9];
    uint32_t counter = 1;
    DirectoryEntry read_entry;
    
    print("\n    No   Name        Ext    Size      Creation time");
    for(uint32_t i = 0; i < directory_reader.cluster_count; i++){
        for(uint16_t j = 1; j < SECTOR_COUNT; j++){
            read_entry = directory_reader.content[i].entry[j];
            
            if(!is_entry_empty(read_entry)){
                print("\n    ");
                    int_toString(counter, char_buffer);
                    print(char_buffer);
                    print(".");
                    for(int i = strlen(char_buffer); i < 4; i++){
                        print(" ");
                    }

                    for(int j = 0; j < 8; j++){
                        if(read_entry.filename[j] == 0){
                           print_char(' ');
                        }
                        else{
                            print_char(read_entry.filename[j]);
                        }
                    }

                    print("    ");
                    for(int j = 0; j < 3; j++){
                        if(read_entry.extension[j] == 0){
                            print_char(' ');
                        }
                        else print_char(read_entry.extension[j]);
                    }
                    
                    print("    ");
                    int_toString(read_entry.size, char_buffer);
                    print(char_buffer);


                    for(int i = strlen(char_buffer); i < 10; i++){
                        print(" ");
                    }

                    int_toString(read_entry.creation_time_hours, char_buffer);
                    print(char_buffer);
                    print(":");
                    int_toString(read_entry.creation_time_minutes, char_buffer);
                    print(char_buffer);
                    print(":");
                    int_toString(read_entry.creation_time_seconds, char_buffer);
                    print(char_buffer);

                    print("  ");
                    int_toString(read_entry.creation_time_day, char_buffer);
                    print(char_buffer);
                    print("/");
                    int_toString(read_entry.creation_time_month, char_buffer);
                    print(char_buffer);
                    print("/");
                    time current_time = get_time();
                    int_toString((read_entry.creation_time_year) + current_time.century * 100, char_buffer);
                    print(char_buffer);

                    counter++;
            }
        }
    }

    closef_dir(directory_reader);
}
void mkdir(char *dirname, uint32_t currentCluster){
    uint8_t success = 0;
    FAT32DriverRequest req;
    req.name[0] = dirname[0];
    req.name[1] = dirname[1];
    req.name[2] = dirname[2];
    req.name[3] = dirname[3];
    req.name[4] = dirname[4];
    req.name[5] = dirname[5];
    req.name[6] = dirname[6];
    req.name[7] = dirname[7];

    req.parent_cluster_number = currentCluster;
    syscall(SYSCALL_WRITE_FILE, (uint32_t) &req, (uint32_t) &success, currentCluster);

    if(success){
        print("\nDirectory created successfully.");
    }
    else{
        print("\nFailed to create directory.");
    }
}

DirectoryEntry emptyEntry = {0};

// ngeprint smua file di cluster yg sama
// void whereis(uint16_t cluster_number, FAT32DriverRequest* result_array, uint16_t* result_count){
//     uint32_t reader[CLUSTER_SIZE/4] = {0};
//     uint16_t count = 0;
//     uint16_t sector_count = 1;
//     uint32_t current_cluster = cluster_number;
//     DirectoryTable table;  // declare the table variable
//     do{
//         syscall(SYSCALL_READ_CLUSTERS, (uint32_t) reader, (uint32_t) &current_cluster, (uint32_t) &sector_count);
//         syscall(SYSCALL_AS_DIRECTORY, (uint32_t) reader, (uint32_t) &table, 0);
//         for(int i = 1; i < SECTOR_COUNT; i++){

//             if (memcmp(&table.entry[i], &emptyEntry, 32) != 0){
//                 FAT32DriverRequest request;
//                 syscall(SYSCALL_MEMCPY, (uint32_t) &request.name, (uint32_t) &table.entry[i].filename, 8);
//                 syscall(SYSCALL_MEMCPY, (uint32_t) &request.ext, (uint32_t) &table.entry[i].extension, 3);
//                 request.parent_cluster_number = cluster_number;
                
                
//                 syscall(SYSCALL_MEMCPY, (uint32_t) &result_array[count], (uint32_t) &request, sizeof(FAT32DriverRequest));
//                 count++;

//                 if(count >= *result_count){
//                     *result_count = count;
//                     return;
//                 }
//             }
//         } 
//         if (current_cluster != END_OF_FILE) {
//         syscall(SYSCALL_READ_CLUSTERS, (uint32_t) reader, (uint32_t) FAT_CLUSTER_NUMBER, (uint32_t) &sector_count);
//         current_cluster = reader[current_cluster];
//         }
//     } while (current_cluster != END_OF_FILE);

//     *result_count = count;
//     return;
// }

void whereis(uint16_t cluster_number, char* filename, FAT32DriverRequest* result_array, uint16_t* result_count){
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    char buffer[256]; 
    uint16_t count = 0;
    uint16_t sector_count = 1;
    uint32_t current_cluster = cluster_number;
    DirectoryTable table; 
do{
    syscall(SYSCALL_READ_CLUSTERS, (uint32_t) reader, (uint32_t) current_cluster, (uint32_t) sector_count);
    syscall(SYSCALL_AS_DIRECTORY, (uint32_t) reader, (uint32_t) &table, 0);
    for(int i = 1; i < SECTOR_COUNT; i++){
        if (memcmp(&table.entry[i], &emptyEntry, 32) != 0){
            char name[8];
            char ext[3];

            // Extract the name and extension of the file/folder
            memcpy(name, table.entry[i].filename, 8);
            memcpy(ext, table.entry[i].extension, 3);

            // Null-terminate the name and extension strings
            // name[8] = '\0';
            // ext[3] = '\0';

            // Compare name with user input
            if (strcmp(name, filename) == 0){
                FAT32DriverRequest request;
                memcpy(request.name, name, 8);
                memcpy(request.ext, ext, 3);
                request.parent_cluster_number = cluster_number;
                memcpy(&result_array[count], &request, sizeof(FAT32DriverRequest));
                count++;

                if(count >= *result_count){
                    *result_count = count;
                    return;
                }

                // Print the location of the file/folder found
                int_toString(current_cluster, buffer); // update buffer with current cluster number
                // print("File/Folder location: Cluster ");
                // print(buffer);
                // print("\n");

                // // Print some debugging information
                // print("Found file/folder: ");
                // print(name);
                // print(".");
                // print(ext);
                // print(" in cluster ");
                // print(buffer);
                // print("\n");
            }
        }
    } 
    syscall(SYSCALL_READ_CLUSTERS, (uint32_t) reader, (uint32_t) 1, (uint32_t) sector_count);
    current_cluster = reader[current_cluster];

    // Update buffer with current cluster number
    int_toString(current_cluster, buffer);

    // Print some debugging information
    // print("Checking cluster ");
    // print(buffer);
    // print("\n");

} while (current_cluster != END_OF_FILE);
    *result_count = count;
    return;
}



void clear(){
    syscall(SYSCALL_CLEAR_SCREEN, 0, 0, 0);
}

//TODO: isi function command di sini
