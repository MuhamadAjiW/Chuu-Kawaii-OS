#include "lib-header/keyboard.h"
#include "lib-header/framebuffer.h"
#include "lib-header/stdtype.h"
#include "lib-header/string.h"
#include "lib-header/shell.h"
#include "lib-header/fat32.h"
#include "lib-header/disk.h"
#include "lib-header/parser.h"
#include "lib-header/stdmem.h"

char currentDir[8] = {'r', 'o', 'o', 't',' ', ' ', ' ', ' '};
uint16_t currentCluster = 2;

extern DirectoryEntry emptyEntry;

void init_shell(){
    clear_reader();
    framebuffer_printDef("\n    >> ");
    framebuffer_set_cursor(1,7);
    framebuffer_set_limit(1,6);
}

void execute_reader(){
    init_parser();
    splitWords();

    if(get_word_count() == 1){
        if(strcmp(get_parsed()[0], "clear") == 0){
            framebuffer_clear();
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
            
            framebuffer_printDef("\n");
            framebuffer_printDef("\n    Parent folder: ");

            for(int j = 0; j < 8; j++){
                char_buffer[j] = table.entry[0].filename[j];
            }
            char_buffer[8] = 0;
            framebuffer_printDef(char_buffer);

            framebuffer_printDef("\n    No  Name        Ext    Size");
            for(int i = 1; i < 64; i++){
                if(memcmp(&table.entry[i], &emptyEntry, 32) != 0){

                    framebuffer_printDef("\n    ");
                    int_toString(counter, char_buffer);
                    framebuffer_printDef(char_buffer);
                    framebuffer_printDef(".  ");

                    for(int j = 0; j < 8; j++){
                        char_buffer[j] = table.entry[i].filename[j];
                    }
                    char_buffer[8] = 0;

                    framebuffer_printDef(char_buffer);

                    for(int j = 0; j < 3; j++){
                        char_buffer[j] = table.entry[i].extension[j];
                    }
                    char_buffer[3] = 0;
                    framebuffer_printDef("    ");
                    framebuffer_printDef(char_buffer);
                    
                    framebuffer_printDef("    ");
                    int_toString(table.entry[i].size, char_buffer);
                    framebuffer_printDef(char_buffer);

                    counter++;
                }
            }
            framebuffer_printDef("\n");
        }
        else{
            framebuffer_printDef("\nYou wrote: ");
            framebuffer_printDef(get_keyboard_buffer());
            framebuffer_printDef("\n");
        }
    }
    else{
        framebuffer_printDef("\nYou wrote: ");
        framebuffer_printDef(get_keyboard_buffer());
        framebuffer_printDef("\n");
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
    uint16_t cursor = framebuffer_get_cursor();
    uint8_t row = cursor/80;

    if(row != 24){
        framebuffer_set_cursor(row + 1, 0);
        framebuffer_printDef("    >> ");
        framebuffer_set_limit(row + 1,6);
        framebuffer_set_cursor(row + 1, 7);
    }
    else{
        framebuffer_set_cursor(row, 0);
        framebuffer_scroll();
        framebuffer_printDef("    >> ");
        framebuffer_set_limit(row,6);
        framebuffer_set_cursor(row, 7);
    }
}

