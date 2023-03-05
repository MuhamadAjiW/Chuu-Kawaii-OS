
#include "../lib-header/memory_manager.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/stdmem.h"


uint32_t last_alloc = 0;
uint32_t heap_start = 0;
uint32_t heap_end = 0;

void initialize_memory(){
    last_alloc = 0x30000000; //start alignment + 2mb untuk kernel
    heap_start = last_alloc;
    heap_end = 0x40000000; // 256mb heap
    memset((char*) heap_start, 0, heap_end - heap_start);
}

char* malloc(uint32_t size){
    uint8_t* memory = (uint8_t*) heap_start;

    if (size == 0){
        return 0;
    }
    else{
        while((uint32_t) memory < last_alloc){
            allocator* a = (allocator*) memory;

            if(a->status){
                memory += a->size;
                memory += sizeof(allocator);
                memory += 4;
            }
            else{
                if(a->size >= size){
                    a->status = 1;
                    a->size = size;
                    memset(memory + sizeof(allocator), 0, size);
                    return (char*)(memory + sizeof(allocator));
                }
                else{
                    memory += a->size;
                    memory += sizeof(allocator);
                    memory += 4;
                }
            }
        }
    }

    if (last_alloc + size + sizeof(allocator) >= heap_end){
        return 0;
    }
    else{
        allocator* alloc = (allocator*) last_alloc;
        alloc->status = 1;
        alloc->size = size;

        last_alloc += size;
        last_alloc += sizeof(allocator);
        last_alloc += 4;
        memset((char*)((uint32_t)alloc + sizeof(allocator)), 0, size);
        return (char*)((uint32_t)alloc + sizeof(allocator));
    }

}

void free(void* memory){
    allocator* alloc = (memory - sizeof(allocator));
    alloc->status = 0;
}

