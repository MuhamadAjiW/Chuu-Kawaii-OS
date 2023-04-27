#include "../lib-header/commands-util.h"

#include "../lib-header/stdio.h"
#include "../lib-header/string.h"
#include "../lib-header/time.h"
#include "../lib-header/stdlib.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/syscall.h"

#include "../lib-header/parserpath.h"
#include "../lib-header/user-shell.h"

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

uint8_t is_directorypath_valid(char* pathname, uint32_t current_cluster){
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    DirectoryTable table = {0};
    parse_path(pathname);
    uint8_t counter = 0;
    uint8_t isFound = 0;
    

    if (strcmp(get_parsed_path_result()[0], "root") == 0){
        current_cluster = 2;
        counter = 1;
    } 

    for(int j = counter; j < get_parsed_path_word_count(); j++){
        isFound = 0;
        do {
            readcluster((void*)reader, current_cluster, 1);
            table = asdirectory(reader);
            for(int i= 1; i < SECTOR_COUNT; i++){
                if ((memcmpr(&table.entry[i].filename, get_parsed_path_result()[j], 8) == 0)){
                    // print("\n");
                    // print(table.entry[i].filename);
                    if (isdirectory(table.entry[i].cluster_number)){
                        current_cluster = table.entry[i].cluster_number;
                        isFound = 1;
                        print("\nfound!");
                        break;
                    }
                }
                }
            
            readcluster((void*)reader, FAT_CLUSTER_NUMBER, 1);
            current_cluster = reader[current_cluster];

        } while (current_cluster != END_OF_FILE && isFound == 0);
    }

    parser_path_clear();
    return isFound;
}

uint8_t is_filepath_valid(char* pathname, uint32_t current_cluster){
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    DirectoryTable table = {0};
    parse_path(pathname);
    uint8_t counter = 0;
    uint8_t isFound = 0;
    int pathLength = get_parsed_path_word_count();
    
    if (strcmp(get_parsed_path_result()[0], "root") == 0){
        current_cluster = 2;
        counter = 1;
    }

    for(int j = counter; j < pathLength - 1; j++){ // cari sampe ujung - 1
        isFound = 0;
        do {
            readcluster((void*)reader, current_cluster, 1);
            table = asdirectory(reader);
            for(int i= 1; i < SECTOR_COUNT; i++){
                if ((memcmpr(&table.entry[i].filename, get_parsed_path_result()[j], 8) == 0)){
                    if (isdirectory(table.entry[i].cluster_number)){
                        current_cluster = table.entry[i].cluster_number;
                        isFound = 1;
                        print("\nfound!");
                        break;
                    }
                }
            }
            
            readcluster((void*)reader, FAT_CLUSTER_NUMBER, 1);
            current_cluster = reader[current_cluster];
        } while (current_cluster != END_OF_FILE && isFound == 0);
    }

    if (isFound) {
        readcluster((void*)reader, current_cluster, 1);
        table = asdirectory(reader);

        int dotIdx  = -1;
        int fileNameExtLen = strlen(get_parsed_path_result()[pathLength - 1]);
        
        // search .
        for (int i = fileNameExtLen - 1; i >= 0; i--) {
            if (get_parsed_path_result()[pathLength - 1][i] == '.') {
                dotIdx = i;
                break;
            }
        }

        if (dotIdx == -1) { // dot not found, not a valid file name
            isFound = 0;
        } else {
            // split into file name and file ext
            char fileName[8];
            char fileExt[3];

            int minNameLen = 8;
            if (dotIdx < minNameLen) {
                minNameLen = dotIdx;
            }

            int minExtLen = 3;
            if (fileNameExtLen - 1 - dotIdx < minExtLen) {
                minExtLen = fileNameExtLen - dotIdx - 1;
            }

            for (int i = 0; i < minNameLen; i++) {
                fileName[i] = get_parsed_path_result()[pathLength - 1][i];
            }

            for (int i = 0; i < minExtLen; i++) {
                fileExt[i] = get_parsed_path_result()[pathLength - 1][dotIdx + 1 + i];
            }

            for(int i = 1; i < SECTOR_COUNT; i++){
                if ((memcmpr(&table.entry[i].filename, fileName, 8) == 0) && (memcmpr(&table.entry[i].extension, fileExt, 3) == 0)){
                    if (!isdirectory(table.entry[i].cluster_number)){
                        current_cluster = table.entry[i].cluster_number;
                        isFound = 1;
                        print("\nfound!");
                        break;
                    }
                }
            }
        }
    }

    parser_path_clear();
    return isFound;
}

uint32_t path_to_cluster(char* pathname, uint32_t current_cluster){
    // prekondisi: path sudah valid
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    DirectoryTable table = {0};
    parse_path(pathname);
    uint8_t counter = 0;
    uint8_t isFound = 0;

    if (strcmp(get_parsed_path_result()[0], "root") == 0){
        current_cluster = 2;
        counter = 1;
    } 

    for(int j = counter; j < get_parsed_path_word_count(); j++){
        isFound = 0;
        do {
            readcluster((void*)reader, current_cluster, 1);
            table = asdirectory(reader);
            for(int i= 1; i < SECTOR_COUNT; i++){
                if ((memcmpr(&table.entry[i].filename, get_parsed_path_result()[j], 8) == 0) &&
                    (isdirectory((uint32_t) &table.entry[i].cluster_number))){
                        current_cluster = (uint32_t) &table.entry[i].cluster_number;
                        isFound = 1;
                        break;
                }
            }
            if (!isFound){
                readcluster((void*)reader, FAT_CLUSTER_NUMBER, 1);
                current_cluster = reader[current_cluster];
            }

        } while (current_cluster != END_OF_FILE && isFound == 0);
    }

    parser_path_clear();
    return current_cluster;
}

FAT32DriverRequest path_to_file_request(char* pathname, int current_cluster) {
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    DirectoryTable table = {0};
    int parentCluster = path_to_cluster(pathname, current_cluster);
    parse_path(pathname);
    int pathLength = get_parsed_path_word_count();

    readcluster((void*)reader, current_cluster, 1);
    table = asdirectory(reader);

    int dotIdx  = -1;
    int fileNameExtLen = strlen(get_parsed_path_result()[get_parsed_path_word_count() - 1]);
    
    // search .
    for (int i = fileNameExtLen - 1; i >= 0; i--) {
        if (get_parsed_path_result()[get_parsed_path_word_count() - 1][i] == '.') {
            dotIdx = i;
            break;
        }
    }

    // split into file name and file ext
    char fileName[8];
    char fileExt[3];

    int minNameLen = 8;
    if (dotIdx < minNameLen) {
        minNameLen = dotIdx;
    }

    int minExtLen = 3;
    if (fileNameExtLen - 1 - dotIdx < minExtLen) {
        minExtLen = fileNameExtLen - dotIdx - 1;
    }

    for (int i = 0; i < minNameLen; i++) {
        fileName[i] = get_parsed_path_result()[pathLength - 1][i];
    }

    for (int i = 0; i < minExtLen; i++) {
        fileExt[i] = get_parsed_path_result()[pathLength - 1][dotIdx + 1 + i];
    }

    for(int i = 1; i < SECTOR_COUNT; i++){
        if ((memcmpr(&table.entry[i].filename, fileName, 8) == 0) && (memcmpr(&table.entry[i].extension, fileExt, 3) == 0)){
            if (!isdirectory(table.entry[i].cluster_number)){
                current_cluster = table.entry[i].cluster_number;
                break;
            }
        }
    }

    FAT32DriverRequest req = {
        .parent_cluster_number = parentCluster,
        .buffer_size = 0
    };
    memcopy(req.name, fileName, 8);
    memcopy(req.ext, fileExt, 3);

    parser_path_clear();

    return req;
}

FAT32DriverRequest path_to_dir_request(char* pathname, int current_cluster) {
    parse_path(pathname);
    int parentCluster = path_to_cluster(pathname, current_cluster);
    FAT32DriverRequest req = {
        .parent_cluster_number = parentCluster,
        .buffer_size = 0
    };

    memcopy(req.name, get_parsed_path_result()[get_parsed_path_word_count() - 1], 8);

    parser_path_clear();

    return req;
}

FAT32DirectoryReader get_self_dir_info(uint32_t current_cluster){
    FAT32DirectoryReader retval;
    syscall(SYSCALL_SELF_DIR_INFO, current_cluster, (uint32_t) &retval, 0);
    return retval;
}