#include "../lib-header/string.h"
#include "../lib-header/stdmem.h"

char** split_reader;
int word_count;

// Kalo split jangan lupa clear, memory leak ntar
void clear_parser(){
    for(int i = 0; i < word_count; i++){
        free(split_reader[i]);
    }
    free(split_reader);
    //main_parser[0] = 0;
    word_count = 0;
}

void split_words(char* main_parser){
    int index = 0;
    int letter_counter = 0;
    word_count = 0;
    while (main_parser[index] != 0){
        if(main_parser[index] == ' '){
            index++;
            continue;
        }
        else{
            while (main_parser[index] != ' ' && main_parser[index] != 0 && main_parser[index] != '\n'){
                index++;
            }

            word_count++;
        }
    }
    split_reader = (char**) malloc(sizeof(char*) * word_count);

    index = 0;
    letter_counter = 0;
    word_count = 0;
    while (main_parser[index] != 0){
        if(main_parser[index] == ' '){
            index++;
            continue;
        }
        else{
            letter_counter = 0;

            while (main_parser[index] != ' ' && main_parser[index] != 0 && main_parser[index] != '\n'){
                letter_counter++;
                index++;
            }
            split_reader[word_count] = (char*) malloc(sizeof(char) * letter_counter + 1);

            for(int i = 0; i < letter_counter; i++){
                split_reader[word_count][i] = main_parser[index - letter_counter + i];
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