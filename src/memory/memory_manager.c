
#include "../lib-header/memory_manager.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/paging.h"

static uint32_t last_alloc = 0;
static uint32_t heap_start = 0;
static uint32_t heap_end = 0;

static uint32_t dynamic_pointers = 0;

//heap
void initialize_memory(){
    struct PageDirectoryEntryFlag flags ={
        .present_bit       = 1,
        .user_supervisor   = 1,
        .write_bit         = 1,
        .use_pagesize_4_mb = 1
    };
    
    for (int i = 0; i < 4; i++)    {
        update_page_directory_entry((void *)(0x800000 + (i*0x400000)), (void *)(0xc0400000 + (i*0x400000)), flags);
    }

    last_alloc = 0xc0400000; //start alignment
    heap_start = last_alloc;
    heap_end = 0xc1400000; // 16mb heap
    memset((char*) heap_start, 0, heap_end - heap_start);
}

void clean_memory(){
    memset((char*) heap_start, 0, last_alloc - heap_start);
    last_alloc = heap_start;
}

void* malloc(uint32_t size){
    void* memory = (void*) heap_start;

    if (size == 0){
        return 0;
    }
    else{
        while((uint32_t) memory < last_alloc){
            allocator* a = (allocator*) memory;

            if(a->status){
                memory += a->size;
                memory += sizeof(allocator);
            }
            else{
                if(a->size >= size + sizeof(allocator) || a->size == size){
                    uint32_t oldsize = a->size;
                    a->status = 1;
                    a->size = size;

                    memset(memory + sizeof(allocator), 0, size + sizeof(allocator));

                    if (oldsize != size){
                        a = (allocator*) ((uint32_t) a + sizeof(allocator) + size);
                        a->status = 0;
                        a->size = oldsize - size - sizeof(allocator);                        
                    }

                    dynamic_pointers++;
                    
                    return (void*)(memory + sizeof(allocator));
                }
                else{
                    memory += a->size;
                    memory += sizeof(allocator);
                }
            }
        }
    }

    if (last_alloc + size + sizeof(allocator) >= heap_end){
        __asm__ volatile ("int $4");
        return 0;
    }
    else{
        allocator* alloc = (allocator*) last_alloc;
        alloc->status = 1;
        alloc->size = size;

        last_alloc += size;
        last_alloc += 2*sizeof(allocator);
        memset((char*)((uint32_t)alloc + sizeof(allocator)), 0, size + sizeof(allocator));

        dynamic_pointers++;

        return (void*)((uint32_t)alloc + sizeof(allocator));
    }
}

void* realloc(void* ptr, uint32_t size){
    allocator* alloc = (allocator*)((uint32_t)ptr - sizeof(allocator));
    uint32_t oldsize = alloc->size;

    void* newptr = malloc(size);

    if (oldsize > size) memcpy(newptr, ptr, size);
    else memcpy(newptr, ptr, oldsize);
    
    free(ptr);

    return newptr;
}



void free(void* memory){
    allocator* alloc = (memory - sizeof(allocator));
    alloc->status = 0;
    uint32_t oldsize = alloc->size;

    alloc += sizeof(allocator) + oldsize;
    alloc->status = 0;
    alloc->size = 0;

    dynamic_pointers--;

    if(dynamic_pointers == 0) clean_memory();
}
