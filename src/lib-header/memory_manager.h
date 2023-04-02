
#ifndef _MEMMNG_H
#define _MEMMNG_H

#include "stdtype.h"

struct allocator{
    bool status;
    uint32_t size;
};
typedef struct allocator allocator;

void initialize_memory();
void clean_memory();


void* malloc(uint32_t size);
void* realloc(void* ptr, uint32_t size);
void free(void*);


#endif