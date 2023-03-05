
#ifndef _MEMMNG_H
#define _MEMMNG_H

#include "lib-header/stdtype.h"

struct allocator{
    bool status;
    uint32_t size;
};
typedef struct allocator allocator;

void initialize_memory();

char* malloc(uint32_t size);
void free();

#endif