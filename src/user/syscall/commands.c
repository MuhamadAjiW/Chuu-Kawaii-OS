
#include "../lib-header/commands.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/stdio.h"
#include "../lib-header/string.h"
#include "../lib-header/time.h"
#include "../lib-header/stdlib.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/syscall.h"
#include "../lib-header/commands-util.h"

#include "../lib-header/parserpath.h"
#include "../lib-header/parser.h"
#include "../lib-header/user-shell.h"
#include "../lib-header/commands-util.h"

#define ANIM_FRAMECOUNT 29

void delay(uint32_t ms){
    uint32_t currentTick = 0;
    uint32_t cachedTick = 0;
    syscall(SYSCALL_GETTICK, (uint32_t) &currentTick, 0, 0);
    cachedTick = currentTick + ms;

    while (currentTick < cachedTick){
        syscall(SYSCALL_GETTICK, (uint32_t) &currentTick, 0, 0);
    }
}

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

    print("\nDirectory created successfully.");
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
            if (memcmp(name, filename, 8) == 0){
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

                int_toString(current_cluster, buffer); // update buffer with current cluster number

            }
        }
    } 
    syscall(SYSCALL_READ_CLUSTERS, (uint32_t) reader, (uint32_t) 1, (uint32_t) sector_count);
    current_cluster = reader[current_cluster];

    // Update buffer with current cluster number
    int_toString(current_cluster, buffer);


} while (current_cluster != END_OF_FILE);
    *result_count = count;
    return;
}



void clear(){
    syscall(SYSCALL_CLEAR_SCREEN, 0, 0, 0);
}

void ls(uint32_t currentCluster){
    FAT32DirectoryReader directory_reader;

    directory_reader = get_self_dir_info(currentCluster);
    // char char_buffer[9];
    // uint32_t counter = 1;
    DirectoryEntry read_entry;
    print("\nfiles and directories: \n");

    for(uint32_t i = 0; i < directory_reader.cluster_count; i++){
        for(uint16_t j = 1; j < SECTOR_COUNT; j++){
            read_entry = directory_reader.content[i].entry[j];
            
            if(!is_entry_empty(read_entry)){
                int buff = 0;
                for(int j = 0; j < 8; j++){
                    if(read_entry.filename[j] == 0){
                        print_char(' ');
                    }
                    else{
                        print_char(read_entry.filename[j]);
                        buff++;
                    }
                }

                // cleaning things out
                for(int k = buff; k < 8; k++){
                    print_char(' ');
                    }
            }
        }
    }
}

directory_info cd(char* pathname, directory_info current_dir){
    print("\nmulai masuk cd");
    print(pathname);
    print("\n");
    directory_info new_dir = {
        .cluster_number = path_to_cluster(pathname, current_dir.cluster_number)
    };
    print("\nsebelum memcopy");
    memcopy(new_dir.directory_path, pathname, 255);
    print("\nsetelah memcopy");
    return new_dir;
}

void rm(uint32_t currentCluster) {
    int length = get_parsed_word_count();

    if (length >= 2) {
        if (is_directorypath_valid(get_parsed_result()[length - 1], currentCluster)) {
            // TODO [minor]: check dir kosong / ngga
            uint8_t isEmptyDir = 0; 
            if (strcmp(get_parsed_result()[1],"-r") != 0 && !isEmptyDir) { // flag -r selalu di tengah
                print("\nrm: cannot remove '");
                print(get_parsed_result()[length - 1]);
                print("': Is a directory");
            } else {
                deletef(path_to_dir_request(get_parsed_result()[length - 1], currentCluster));
            }
        } else if (is_filepath_valid(get_parsed_result()[length - 1], currentCluster)) {
            deletef(path_to_file_request(get_parsed_result()[length - 1], currentCluster));
        } else {
            print("\nrm: Invalid command");
        }
    } else {
        print("\nrm: Invalid command");
    }
}

void cp(uint32_t currentCluster) {
    // if (get_parsed_word_count() >= 3) {
    //     uint8_t isdir = 0;
    //     uint8_t isfile = 0;

    //     FAT32DriverRequest srcs[get_parsed_word_count() - 2];
    //     for (int i = 1; i < get_parsed_word_count() - 1; i++) {
    //         isdir = is_directorypath_valid(get_parsed_result()[i]);
    //         isfile = is_filepath_valid(get_parsed_result()[i]);
    //         if (!isdir && !isfile) {
    //             print("\ncp: Invalid path");
    //             return;
    //         } else {
    //             FAT32DriverRequest temp;

    //             srcs[i - 1] = 
    //         }
    //     }

    //     // semua source valid
    //     isdir = is_directorypath_valid(get_parsed_result()[get_parsed_word_count() - 1]);
    //     isfile = is_filepath_valid(get_parsed_result()[get_parsed_word_count() - 1]);

    //     if (get_parsed_word_count() > 3) {
    //         if (isfile) {
    //             print("\ncp: Destination is not a directory");
    //         } else if (isdir) {
    //             // 
    //         } else {
    //             print("\ncp: Destination is not a directory");
    //         }
    //     } else {

    //     }
    // } else {
    //     print("\ncp: Invalid command");
    // }
    // dummy
    FAT32DriverRequest src = path_to_file_request(get_parsed_result()[1], currentCluster);
    FAT32FileReader result = readf(src);

    struct FAT32DriverRequest dest = {
        .name                  = "12345678",
        .ext                   = "abc",
        .parent_cluster_number = ROOT_CLUSTER_NUMBER,
        .buffer_size           = 0,
    };

    copy1(result, dest);
}

void mv(uint32_t currentCluster) {
    cp(currentCluster);
    rm(currentCluster);
}

void copy1(FAT32FileReader readed, FAT32DriverRequest destFile) {
    destFile.buffer_size = readed.cluster_count * CLUSTER_SIZE;
    struct ClusterBuffer cbuf[destFile.buffer_size];

    for (uint32_t i = 0; i < destFile.buffer_size; i++)
        for (uint32_t j = 0; j < CLUSTER_SIZE; j++)
            cbuf[i].buf[j] = readed.content[i].buf[j];

    destFile.buf = cbuf;

    writef(destFile);
}

void cat(uint32_t currentCluster) {
    // prekondisi: path sudah valid, dan adalah path ke file
    FAT32DriverRequest req = path_to_file_request(get_parsed_result()[1], currentCluster);
    // print("\n------------- GADIPAKE --------------\n");
    // print("\n");
    // print(req.name);
    // print(req.ext);
    // char cluster[10];
    // int_toString(req.parent_cluster_number, cluster);
    // print(cluster);
    
    FAT32FileReader result = readf(req);
    
    // char testing[10];
    // int_toString((int)result.clsuster_count, testing);
    // print(testing);
    // print("\n\n-------------------\n\n");
    for (int i = 0; i < (int) result.cluster_count; i++) {
        for (int j = 0; j < CLUSTER_SIZE; j++) {
            print_char((char) result.content[i].buf[j]);
        }
    }
} 