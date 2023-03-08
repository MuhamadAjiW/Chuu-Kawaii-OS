#include "lib-header/keyboard.h"
#include "lib-header/stdtype.h"
#include "lib-header/string.h"
#include "lib-header/shell.h"
#include "lib-header/fat32.h"
#include "lib-header/disk.h"
#include "lib-header/parser.h"
#include "lib-header/stdmem.h"
#include "lib-header/portio.h"
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
            ClusterBuffer cluster_reader;
            DirectoryTable table;
            read_clusters(&cluster_reader, currentCluster, 1);

            void* runner = (void*) &cluster_reader;
            table = read_directory(runner);
            
            int counter = 1;
            char char_buffer[9];
            
            graphics_print("\n");
            graphics_print("\n    Parent folder: ");

            for(int j = 0; j < 8; j++){
                char_buffer[j] = table.entry[0].filename[j];
            }
            char_buffer[8] = 0;
            graphics_print(char_buffer);

            graphics_print("\n    No  Name        Ext    Size");
            for(int i = 1; i < 64; i++){
                if(memcmp(&table.entry[i], &emptyEntry, 32) != 0){

                    graphics_print("\n    ");
                    int_toString(counter, char_buffer);
                    graphics_print(char_buffer);
                    graphics_print(".  ");

                    for(int j = 0; j < 8; j++){
                        char_buffer[j] = table.entry[i].filename[j];
                    }
                    char_buffer[8] = 0;

                    graphics_print(char_buffer);

                    for(int j = 0; j < 3; j++){
                        char_buffer[j] = table.entry[i].extension[j];
                    }
                    char_buffer[3] = 0;
                    graphics_print("    ");
                    graphics_print(char_buffer);
                    
                    graphics_print("    ");
                    int_toString(table.entry[i].size, char_buffer);
                    graphics_print(char_buffer);

                    counter++;
                }
            }
            graphics_print("\n");
        }
        else{
            graphics_print("\nYou wrote: ");
            graphics_print(get_keyboard_buffer());
            graphics_print("\n");
        }
    }
    else if (get_word_count() != 0){
        graphics_print("\nYou wrote: ");
        graphics_print(get_keyboard_buffer());
        graphics_print("\n");
    }
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

