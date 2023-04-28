
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
    FAT32DriverRequest req = {0};

    for(uint8_t i = 0; i < 8; i++){
        if(dirname[i] == 0){
            break;
        }
        req.name[i] = dirname[i];
    }

    req.parent_cluster_number = currentCluster;
    req.buffer_size = 0;
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

void whereis(uint16_t current_cluster, char* filename, char* path){
    char fileName[8] = {0};
    char fileExt[3] = {0};

    int dotIdx  = -1;
    int fileNameExtLen = strlen(filename);
    
    // search .
    for (int i = fileNameExtLen - 1; i >= 0; i--) {
        if (filename[i] == '.') {
            dotIdx = i;
            break;
        }
    }

    if (dotIdx == -1) { // dot not found, not a valid file name
        for (int i = 0; i < 8; i++) {
            if(filename[i] == 0){
                break;
            }
            fileName[i] = filename[i];
        }
    } else {
        // split into file name and file ext
        int minNameLen = 8;
        if (dotIdx < minNameLen) {
            minNameLen = dotIdx;
        }

        for (int i = 0; i < minNameLen; i++) {
            if(filename[i] == 0){
                break;
            }
            fileName[i] = filename[i];
        }

        int minExtLen = 3;
        if (fileNameExtLen - 1 - dotIdx < minExtLen) {
            minExtLen = fileNameExtLen - dotIdx - 1;
        }
        for (int i = 0; i < minExtLen; i++) {
            if(filename[dotIdx + 1 + i] == 0){
                break;
            }
            fileExt[i] = filename[dotIdx + 1 + i];
        }
    }

    FAT32DirectoryReader read;
    char emptyString[9] = {0};
    char string[9] = {0};
    char extstring[4] = {0};


    read = get_self_dir_info(current_cluster);
    for(uint32_t k = 0; k < read.cluster_count; k++){
        for(uint8_t i= 1; i < SECTOR_COUNT; i++){
            memcpy(string, emptyString, 8);
            memcpy(string, &read.content[k].entry[i].filename, 8);
            memcpy(extstring, emptyString, 3);
            memcpy(extstring, &read.content[k].entry[i].extension, 3);
            if (read.content[k].entry[i].directory){
                
                uint8_t lencounter = 0;
                while(lencounter < 8 && read.content[k].entry[i].filename[lencounter] != 0){
                    lencounter++;
                }

                char* appended = (char*) malloc(strlen(path) + lencounter + 2);
                
                uint8_t lencounter2 = 0;
                while (lencounter2 < strlen(path)){
                    appended[lencounter2] = path[lencounter2];
                    lencounter2++;
                }
                appended[lencounter2] = '/';
                lencounter2++;

                for(uint8_t l = 0; l < lencounter; l++){
                    appended[lencounter2] = read.content[k].entry[i].filename[l];
                    lencounter2++;
                }
                appended[lencounter2] = '\0';


                whereis(read.content[k].entry[i].cluster_number, filename, appended);
                free(appended);
            }

            if ((memcmp(string, fileName, 8) == 0) && (memcmp(extstring, fileExt, 3) == 0)){
                print("\n    ");
                print(path);
                print("/");
                print(filename);
            }

        }
    }
    closef_dir(read);
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
    print("\nsebelum memcpy");
    memcpy(new_dir.directory_path, pathname, 255);
    print("\nsetelah memcpy");
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
    int len = get_parsed_word_count();
    if (len >= 3) { // cp minimal len cmd 3
        uint8_t isdir = 0;
        uint8_t isfile = 0;
        uint8_t hasR = 0;
        uint8_t hasDir = 0;

        FAT32DriverRequest srcs[len - 2]; // create arr of sources
        int nSrc = 0;
        for (int i = 1; i < len - 1; i++) {
            isdir = is_directorypath_valid(get_parsed_result()[i], currentCluster);
            isfile = is_filepath_valid(get_parsed_result()[i], currentCluster);
            if (strcmp(get_parsed_result()[i], "-r") == 0) {
                hasR = 1;
            } else if (isfile) {
                srcs[i - 1] = path_to_file_request(get_parsed_result()[i], currentCluster);
                nSrc++;
            } else if (isdir) {
                srcs[i - 1] = path_to_dir_request(get_parsed_result()[i], currentCluster);
                hasDir = 1;
                nSrc++;
            } else {
                print("\ncp: Invalid path");
                return;
            }
        }

        // semua source valid

        // cek apakah dest ada
        isdir = is_directorypath_valid(get_parsed_result()[len - 1], currentCluster);
        isfile = is_filepath_valid(get_parsed_result()[len - 1], currentCluster);

        if (nSrc > 1) {
            if (isfile) {
                print("\ncp: Destination is not a directory");
            } else if (isdir) {
                // FAT32DriverRequest dest = path_to_dir_request(get_parsed_result()[len - 1], currentCluster);
                for (int i = 0; i < nSrc; i++) {
                    // copyAll(srcs[i], dest);
                    // copy src [file/folder] ke folder
                }
            } else { // ga ketemu
                print("\ncp: Destination is not a directory");
            }
        } else {
            if (isfile) {
                print("\ncp: Cannot overwrite existing file");
            } else if (isdir) {
                // ini kalo 1 file ke 1 folder
                // FAT32DriverRequest dest = path_to_dir_request(get_parsed_result()[len - 1], currentCluster);
                // copy src [file/folder] ke folder

            } else { // ga ketemu 
                if (hasR && hasDir) {
                    //mkdir(get_parsed_result()[len - 1], currentCluster);
                    // FAT32DriverRequest dest = path_to_dir_request(get_parsed_result()[len - 1], currentCluster);

                    // copyAll(srcs[i], dest);
                    FAT32DriverRequest src = srcs[1];
                    FAT32DriverRequest dest = path_to_dir_request(get_parsed_result()[len - 1], currentCluster);
                    copy1Folder(src, dest);
                } else if (hasDir) {
                    print("\ncp: Source is a directory");
                } else {
                    // write file baru
                    FAT32DriverRequest src = srcs[0];
                    FAT32DriverRequest dest = path_to_file_request(get_parsed_result()[len - 1], currentCluster);
                    copy1File(src, dest);
                }
            }
        }
    } else {
        print("\ncp: Invalid command");
    }

}

void mv(uint32_t currentCluster) {
    cp(currentCluster);
    rm(currentCluster);
}

void copy1Folder(FAT32DriverRequest src, FAT32DriverRequest dest) {
    dest.buffer_size = 0;
    writef(dest);

    FAT32DirectoryReader read = get_self_dir_info(dest.parent_cluster_number);
    DirectoryEntry self;
    for(uint32_t i = 0; i < read.cluster_count; i++){
        for(uint32_t j = 1; j < SECTOR_COUNT; j++){
            // read.content[i].entry[j];
            if(memcmp(&read.content[i].entry[j].filename, dest.name, 8) == 0 &&
                memcmp(&read.content[i].entry[j].extension, dest.ext, 3) == 0
            ){
                self = read.content[i].entry[j];
                break;
            }
        }
    }
    closef_dir(read);

    read = readf_dir(src);

    for(uint32_t i = 0; i < read.cluster_count; i++){
        for(uint32_t j = 1; j < SECTOR_COUNT; j++){
            // read.content[i].entry[j];
            if(memcmp(&read.content[i].entry[j], &emptyEntry, 32) == 0){
                continue;
            }

            if(read.content[i].entry[j].directory){
                FAT32DriverRequest newDest = {0};

                memcpy(newDest.name, read.content[i].entry[j].filename, 8);
                newDest.parent_cluster_number = self.cluster_number;

                FAT32DriverRequest newSrc = {0};
                memcpy(newSrc.name, read.content[i].entry[j].filename, 8);
                newSrc.parent_cluster_number = self.cluster_number;
                newSrc.buffer_size = 0;
                
                copy1Folder(newSrc, newDest);
            }
            else{
                FAT32DriverRequest newDest = {
                    .parent_cluster_number = self.cluster_number,
                    .buffer_size = read.content[i].entry[j].size
                };
                
                memcpy(newDest.name, self.filename, 8);
                memcpy(newDest.ext, self.extension, 3);

                FAT32DriverRequest newSrc = {
                    .parent_cluster_number = read.content[i].entry[j].cluster_number,
                    .buffer_size = read.content[i].entry[j].size
                };

                memcpy(newSrc.name, read.content[i].entry[j].filename, 8);
                memcpy(newSrc.ext, read.content[i].entry[j].extension, 3);
                
                copy1File(newSrc, newDest);
            }

        }
    }
    
    closef_dir(read);
}

void copy1File(FAT32DriverRequest src, FAT32DriverRequest dest) {
    FAT32FileReader read = readf(src);
    dest.buffer_size = read.cluster_count * CLUSTER_SIZE;
    dest.buf = read.content;
    writef(dest);
    
    closef(read);
}

void cat(uint32_t currentCluster) {
    // prekondisi: path sudah valid, dan adalah path ke file
    FAT32DriverRequest req = path_to_file_request(get_parsed_result()[1], currentCluster);
    
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