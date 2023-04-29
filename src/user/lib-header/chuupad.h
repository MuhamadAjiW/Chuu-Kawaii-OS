
#ifndef _CHUUPAD_H
#define _CHUUPAD_H

/**
 *  Singleton struct to store app reader information
 *  
 *  @param text_buffer          dynamic buffer for stored text
 *  @param buffersize           current keyboard_buffer size
 *  @param minIdx               min idx of char displayed on screen
 *  @param currentIdx           location of cursor relative to the reader
 */
struct text_reader
{
    char* text_buffer;
    uint32_t buffersize;
    uint32_t minIdx;
    uint32_t currentIdx;
    uint32_t len;
    uint32_t cluster_number;
    char screen_buffer[25*64 + 1];
};
typedef struct text_reader text_reader;

void reader_main();
void reader_with_file(uint32_t cluster_number);
void reader_no_file();

#endif