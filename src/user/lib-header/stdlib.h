
#ifndef _STDMEM_H
#define _STDMEM_H

#include "stdtype.h"

void* malloc(uint32_t size);
void* realloc(void* ptr, uint32_t size);
void free(void*);


#endif