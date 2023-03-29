
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

void enable_paging(void* page_dir);
void virt_to_phys(uint32_t virt, uint32_t phys);
void map_table(uint32_t virt, uint32_t phys, uint32_t flags);
void map_page(uint32_t virt, uint32_t phys, uint32_t flags);
void initialize_paging();

#endif