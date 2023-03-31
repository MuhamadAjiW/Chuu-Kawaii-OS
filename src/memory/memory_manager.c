
#include "../lib-header/memory_manager.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/paging.h"

uint32_t* kernel_dir;
uint32_t* page_directory = 0;
uint32_t* page_table_addr = 0;

uint32_t last_alloc = 0;
uint32_t heap_start = 0;
uint32_t heap_end = 0;

extern struct PageDirectory _paging_kernel_page_directory;

//heap
void initialize_memory(){
    struct PageDirectoryEntryFlag flags ={
        .present_bit       = 1,
        .write_bit         = 1,
        .use_pagesize_4_mb = 1,
    };
    
    update_page_directory_entry((void *)(PAGE_FRAME_SIZE), (void *)(0xc0400000), flags);

    last_alloc = 0xc0400000; //start alignment
    heap_start = last_alloc;
    heap_end = 0xc0800000; // 4mb heap
    memset((char*) heap_start, 0, heap_end - heap_start);
}

char* malloc(uint32_t size){
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
                if(a->size >= size){
                    uint32_t oldsize = a->size;
                    a->status = 1;
                    a->size = size;

                    memset(memory + sizeof(allocator), 0, size + sizeof(allocator));

                    a = (allocator*) ((uint32_t) a + sizeof(allocator) + size);
                    a->status = 0;
                    a->size = oldsize - size - sizeof(allocator);
                    
                    return (char*)(memory + sizeof(allocator));
                }
                else{
                    memory += a->size;
                    memory += sizeof(allocator);
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
        last_alloc += 2*sizeof(allocator);
        memset((char*)((uint32_t)alloc + sizeof(allocator)), 0, size + sizeof(allocator));
        return (char*)((uint32_t)alloc + sizeof(allocator));
    }
}

void free(void* memory){
    allocator* alloc = (memory - sizeof(allocator));
    alloc->status = 0;
    uint32_t oldsize = alloc->size;

    alloc += sizeof(allocator) + oldsize;
    alloc->status = 0;
    alloc->size = 0;
}
