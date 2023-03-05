#include "../lib-header/keyboard.h"
#include "../lib-header/string.h"
#include "../lib-header/memory_manager.h"
#include "../lib-header/stdmem.h"

char main_parser[KEYBOARD_BUFFER_SIZE];
char** split_reader;
int word_count;

void init_parser(){
    strcpy(main_parser, get_keyboard_buffer());
    split_reader = (char**) malloc(sizeof(char) * 1000);
    word_count = 0;
}

void clearParser(){
    free(split_reader);
    main_parser[0] = 0;
    word_count = 0;
}

void splitWords(){
    int index = 0;
    int letter_counter = 0;
    word_count = 0;
    while (main_parser[index] != 0){
        if(main_parser[index] == ' '){
            index++;
            continue;
        }
        else{
            letter_counter = 0;
            while (main_parser[index] != ' ' && main_parser[index] != 0 && main_parser[index] != '\n'){
                split_reader[word_count][letter_counter] = main_parser[index];
                letter_counter++;
                index++;
            }
            split_reader[word_count][letter_counter] = 0;
            word_count++;
        }
    }
    return;
}

char** get_parsed(){
    return split_reader;
}

int get_word_count(){
    return word_count;
}