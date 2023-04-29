#include "../lib-header/stdtype.h"
#include "../lib-header/user-shell.h"
#include "../lib-header/syscall.h"
#include "../lib-header/stdlib.h"
#include "../lib-header/stdio.h"
#include "../lib-header/string.h"
#include "../lib-header/commands.h"
#include "../lib-header/commands-util.h"
#include "../lib-header/parserpath.h"
#include "../lib-header/parser.h"

#include "../lib-header/chuupad.h"

static text_reader reader = {
    .text_buffer = (char*) 1,
    .buffersize = 1,
    .minIdx = 1,
    .currentIdx = 1,
    .len = 1,
    .cluster_number = 1,
    .screen_buffer = {0}
};

uint8_t get_cursor_y(){
    uint8_t yloc;
    syscall(SYSCALL_GET_CURSOR_Y, (uint32_t) &yloc ,0,0);
    return yloc;
}
uint8_t get_cursor_x(){
    uint8_t xloc;
    syscall(SYSCALL_GET_CURSOR_X, (uint32_t) &xloc ,0,0);
    return xloc;
}
uint8_t get_text_edge(uint8_t line){
    uint8_t edge;
    syscall(SYSCALL_GET_TEXT_EDGE, (uint32_t) line, (uint32_t) &edge,0);
    return edge;
}

void initialize_reader(uint32_t cluster_number){
    reader.buffersize = CLUSTER_SIZE;
    reader.text_buffer = (char*) malloc (sizeof(char) * CLUSTER_SIZE + 1);
    reader.text_buffer[reader.buffersize] = 0;
    reader.cluster_number = cluster_number;
    reader.currentIdx = 0;
    reader.minIdx = 0;
    reader.len = 0;
    for(uint16_t i = 0; i < 25*64; i++){
        reader.screen_buffer[i] = 0;
    }
}

void close_reader(){
    free(reader.text_buffer);
}


void append_reader(char in){
    if(reader.len < reader.buffersize - 1){
        for(uint32_t i = reader.len; i > reader.currentIdx; i--){
            reader.text_buffer[i] = reader.text_buffer[i-1];
        }
        reader.text_buffer[reader.currentIdx] = in;
        reader.len++;
        reader.currentIdx++;
    }
    else{
        reader.buffersize += CLUSTER_SIZE;
        reader.text_buffer = (char*) realloc(reader.text_buffer, sizeof(char) * reader.buffersize + 1);
        reader.text_buffer[reader.buffersize] = 0;
        append_reader(in);
    }
}
void move_reader(int direction){
    switch (direction){
        case (1):
            reader.currentIdx++;
            break;
        case (2):
            if(get_cursor_y() != 24){
                uint8_t edge = get_text_edge(get_cursor_y() + 1);
                if (edge < get_cursor_x()){
                    reader.currentIdx += edge;
                }
                else{
                    reader.currentIdx += 25;
                }
            }
            break;
        case (-1):
            reader.currentIdx--;
            break;
        case (-2):
            if(get_cursor_y() != 0){
                uint8_t edge = get_text_edge(get_cursor_y() - 1);
                if (edge < get_cursor_x()){
                    reader.currentIdx -= 25 - edge;
                }
                else{
                    reader.currentIdx -= 25;
                }
            }
            break;
        
        default:
            break;
    }
    reader.currentIdx = reader.currentIdx + direction;
    return;
}

void backspace_reader(){
    for(uint32_t i = reader.currentIdx-1; i < reader.len-1; i++){
        reader.text_buffer[i] = reader.text_buffer[i+1];
    }
    if(reader.currentIdx > 0){
        reader.currentIdx--;
    }
    if(reader.len > 0){
        reader.len--;
    }

    return;
}

void load_buffer(){
    for(uint16_t i = 0; i < 25*64; i++){
        reader.screen_buffer[i] = reader.text_buffer[i + reader.minIdx];
    }
}

void display_text(){
    syscall(SYSCALL_DISPLAY_TEXT,  ((uint32_t)reader.screen_buffer + reader.minIdx), 0, 0);
} 

void reader_with_file(uint32_t cluster_number){
    syscall(SYSCALL_LIMIT_CURSOR, 0, 0, 0);    
    initialize_reader(cluster_number); 

    // prekondisi: path sudah valid, dan adalah path ke file
    FAT32DriverRequest req = path_to_file_request(get_parsed_result()[1], cluster_number);
    
    FAT32FileReader result = readf(req);

    for (uint32_t j = 0; j < result.size; j++) {
        append_reader(*(((char*)result.content) + j));
    }
    reader.currentIdx = 0;

    closef(result);
    load_buffer();
    display_text();
    reader_main();
}

void reader_no_file(uint32_t cluster_number){
    syscall(SYSCALL_LIMIT_CURSOR, 0, 0, 0);
    initialize_reader(cluster_number); 
    reader_main();
}

void scroll(int8_t direction){
    if(direction == 1){
        uint8_t counter = 0;
        while(counter < 64){
            if(reader.screen_buffer[counter] == '\n'){
                counter++;
                break;
            }
            counter++;
        }
        move_reader(counter);
        reader.minIdx += counter;
        if(reader.minIdx > reader.len){
            move_reader((-1)*(reader.len-(64*25) - reader.minIdx));
            reader.minIdx = reader.len - (64*25);
        }
    }
    else if(direction == -1){
        if(reader.minIdx != 0){
            uint8_t counter = 0;
            while(counter < 64){
                //TODO: benerin
                if(reader.screen_buffer[64*24 + counter] == '\n'){
                    counter++;
                    break;
                }
                counter++;
            }
            if(counter > reader.minIdx){
                reader.minIdx = 0;
            }
            else{
                move_reader((-1)*(counter));
                reader.minIdx -= counter;
            }
        }
    }
}

void reader_main(){
    uint8_t running = 1;
    char buf[2] = {0, 0};
    while (running) {
        buf[0] = getc();
        
        if(buf[0] == ESC_CHAR){
            running = 0;
        }

        else if(buf[0] == TAB_CHAR){
            append_reader(' ');
            append_reader(' ');
            append_reader(' ');
            append_reader(' ');
        }
        else if(buf[0] == LARROW_CHAR){
            if(move_cursor(-1)){
                move_reader(-1);
            }
            else{
                scroll(-1);
                display_text();
            }
        }
        
        else if(buf[0] == RARROW_CHAR){
            if(move_cursor(1)){
                move_reader(1);

                if(get_cursor_y() == 24){
                    move_cursor(-2);
                    scroll(1);
                    display_text();
                }
            }
        }

        else if(buf[0] == UARROW_CHAR){
            if(move_cursor(-2)){
                move_reader(-2);
            }
            else{
                scroll(-1);
                display_text();
            }
        }

        else if(buf[0] == DARROW_CHAR){
            if(move_cursor(2)){
                move_reader(2);

                if(get_cursor_y() == 24){
                    move_cursor(-2);
                    scroll(1);
                    display_text();
                }
            }
        }

        if (buf[0] == BACKSPACE_CHAR){
            if(backspace()){
                backspace_reader();
            }
        }
                
        else if((buf[0] >= 32 && buf[0] <= 126) || buf[0] == '\n'){
            append_reader(buf[0]);
            load_buffer();
            display_text();
            
            if(buf[0] == '\n'){
                move_cursor(3);
            }
            else move_cursor(1);

            if(get_cursor_y() == 24){
                move_cursor(2);
                scroll(1);
            }
        }
    }
    
    close_reader();
}
