#include "lib-header/keyboard.h"
#include "lib-header/stdtype.h"
#include "lib-header/string.h"
#include "lib-header/shell.h"
#include "lib-header/fat32.h"
#include "lib-header/disk.h"
#include "lib-header/parser.h"
#include "lib-header/stdmem.h"
#include "lib-header/portio.h"
#include "lib-header/cmos.h"
#include "lib-header/graphics.h"

extern int SYSTEM_RUNNING;

char currentDir[8] = {'r', 'o', 'o', 't',' ', ' ', ' ', ' '};
uint16_t currentCluster = 2;

extern DirectoryEntry emptyEntry;

void init_shell(){
    clear_reader();
    graphics_print("\n    >> ");
    graphics_set_limit(get_cursor_x(), get_cursor_y());
}

void execute_reader(){
    init_parser();
    splitWords();

    if(get_word_count() == 1){
        if(strcmp(get_parsed()[0], "clear") == 0){
            graphics_set_cursor(0, 0);
            graphics_clear_buffer();
            clear_reader();
        }
        else if(strcmp(get_parsed()[0], "dir") == 0){
            dir();
        }
        else{
            graphics_print("You wrote: ");
            graphics_print(get_keyboard_buffer());
        }
    }
    else if (get_word_count() != 0){
        graphics_print("You wrote: ");
        graphics_print(get_keyboard_buffer());
    }
    init_shell();
    clearParser();
}

/*
bool closed_sentence(){
    bool closed = 0;

    for(int i = maxIdx-1; i >= 0; i--){
        if(shellReader[i] == ' ' || shellReader[i] == '\n'){}
        else if(shellReader[i] == ';'){
            shellReader[i] = 0;
            return 1;
        }
        else{
            return 0;
        }
    }

    return closed;
}
*/

void newline_shell(){
    uint8_t row = get_cursor_y();
    if(row != 24){
        graphics_set_cursor(0, row + 1);
        graphics_print("    >> ");
        graphics_set_limit(get_cursor_x(), get_cursor_y());
    }
    else{
        graphics_set_cursor(0, row);
        graphics_scroll();
        graphics_print("    >> ");
        graphics_set_limit(get_cursor_x(), get_cursor_y());
    }
}

void dir(){
    //TODO: Refactor
    uint32_t reader[CLUSTER_SIZE/4] = {0};
    DirectoryTable table;
    read_clusters((void*)reader, currentCluster, 1);

    table = read_directory(reader);
    
    int counter = 1;
    char char_buffer[9];
    
    graphics_print("    Parent folder: ");

    for(int j = 0; j < 8; j++){
        graphics_write_char(table.entry[0].filename[j]);
    }
    graphics_print("\n    No   Name        Ext    Size      Creation time");

    uint32_t current_cluster = currentCluster;
    if(current_cluster != END_OF_FILE){
        while (current_cluster != END_OF_FILE){
            read_clusters((void*)reader, current_cluster, 1);
            table = read_directory(reader);
            
            for(int i = 1; i < 64; i++){
                if(memcmp(&table.entry[i], &emptyEntry, 32) != 0){

                    graphics_print("\n    ");
                    int_toString(counter, char_buffer);
                    graphics_print(char_buffer);
                    graphics_print(".");
                    for(int i = strlen(char_buffer); i < 4; i++){
                        graphics_print(" ");
                    }

                    for(int j = 0; j < 8; j++){
                        if(table.entry[i].filename[j] == 0){
                            graphics_write_char(' ');
                        }
                        graphics_write_char(table.entry[i].filename[j]);
                    }

                    graphics_print("    ");
                    for(int j = 0; j < 3; j++){
                        if(table.entry[i].extension[j] == 0){
                            graphics_write_char(' ');
                        }
                        else graphics_write_char(table.entry[i].extension[j]);
                    }
                    
                    graphics_print("    ");
                    int_toString(table.entry[i].size, char_buffer);
                    graphics_print(char_buffer);


                    for(int i = strlen(char_buffer); i < 10; i++){
                        graphics_print(" ");
                    }

                    int_toString(table.entry[i].creation_time_hours, char_buffer);
                    graphics_print(char_buffer);
                    graphics_print(":");
                    int_toString(table.entry[i].creation_time_minutes, char_buffer);
                    graphics_print(char_buffer);
                    graphics_print(":");
                    int_toString(table.entry[i].creation_time_seconds, char_buffer);
                    graphics_print(char_buffer);

                    graphics_print("  ");
                    int_toString(table.entry[i].creation_time_day, char_buffer);
                    graphics_print(char_buffer);
                    graphics_print("/");
                    int_toString(table.entry[i].creation_time_month, char_buffer);
                    graphics_print(char_buffer);
                    graphics_print("/");
                    int_toString((table.entry[i].creation_time_year) + get_cmos_data().century * 100, char_buffer);
                    graphics_print(char_buffer);

                    counter++;
                }
            }

            read_clusters((void*)reader, FAT_CLUSTER_NUMBER, 1);
            current_cluster = reader[current_cluster];
        }
    }           
}