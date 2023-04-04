#include "../lib-header/string.h"
#include "../lib-header/stdlib.h"
#include "../lib-header/syscall.h"

static char** parsed = (char**) 1;
static int parsed_word_count = 1;

// Kalo split jangan lupa clear, memory leak ntar
void parser_clear(){
    if(parsed != 0){
        for(int i = 0; i < parsed_word_count; i++){
            free(parsed[i]);
        }

        free(parsed);
    }
    //string[0] = 0;
    parsed_word_count = 0;

    return;
}

void parse(char* string){
    int index = 0;
    int letter_counter = 0;
    char* intermediary;
    parsed_word_count = 0;
    while (string[index] != 0){
        if(string[index] == ' '){
            index++;
            continue;
        }
        else{
            while (string[index] != ' ' && string[index] != 0 && string[index] != '\n'){
                index++;
            }

            parsed_word_count++;
        }
    }
    parsed = (char**) malloc(sizeof(char*) * parsed_word_count);

    index = 0;
    letter_counter = 0;
    parsed_word_count = 0;
    while (string[index] != 0){
        if(string[index] == ' '){
            index++;
            continue;
        }
        else{
            letter_counter = 0;

            while (string[index] != ' ' && string[index] != 0 && string[index] != '\n'){
                letter_counter++;
                index++;
            }
            intermediary = (char*) malloc(sizeof(char) * letter_counter + 1);
            parsed[parsed_word_count] = intermediary;

            for(int i = 0; i < letter_counter; i++){
                parsed[parsed_word_count][i] = string[index - letter_counter + i];
            }
            
            parsed[parsed_word_count][letter_counter] = 0;
            parsed_word_count++;
        }
    }
    
    return;
}

char** get_parsed_result(){
    return parsed;
}


int get_parsed_word_count(){
    return parsed_word_count;
}
