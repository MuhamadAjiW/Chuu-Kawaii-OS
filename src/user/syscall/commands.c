
#include "../lib-header/commands.h"

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
    directory_info new_dir = {
        .cluster_number = path_to_cluster(pathname, current_dir.cluster_number)
    };
    memcopy(new_dir.directory_path, pathname, 255);
    return new_dir;
}

void rm(int currentCluster) {
    int length = get_parsed_word_count();

    if (length >= 2) {
        if (is_directorypath_valid(get_parsed_result()[length - 1], currentCluster)) {
            if (strcmp(get_parsed_result()[1],"-r") != 0) { // flag -r selalu di tengah
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
    }
}