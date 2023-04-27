
#include "../lib-header/commands.h"

#include "../lib-header/stdio.h"
#include "../lib-header/string.h"
#include "../lib-header/time.h"
#include "../lib-header/stdlib.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/syscall.h"

#include "../lib-header/parserpath.h"

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

uint32_t cd(char* pathname, uint32_t current_cluster){
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
