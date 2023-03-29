
#include "../lib-header/memory_manager.h"
#include "../lib-header/stdtype.h"
#include "../lib-header/stdmem.h"

uint32_t* page_directory = 0;
uint32_t* page_table_addr = 0;

uint32_t last_alloc = 0;
uint32_t heap_start = 0;
uint32_t heap_end = 0;

void initialize_memory(){
    last_alloc = 0x30000000; //start alignment
    heap_start = last_alloc;
    heap_end = 0x32000000; // 32mb heap
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

void enable_paging(void* page_dir){
    __asm__ volatile ("mov %%eax, %%cr3": :"a"(page_dir));
    __asm__ volatile ("mov %cr0, %eax");
    
    __asm__ volatile ("or 0x80000000, %eax");
    __asm__ volatile ("mov %eax, %cr0");
}



void map_table(uint32_t virt, uint32_t phys, uint32_t flags){
    uint32_t pd_idx = virt >> 22;

    if(page_directory[pd_idx] == 2){
        for(int i = 0; i < 1024; i++){
            page_table_addr[i] = phys | (flags & 0xfff) | 0x01;
            phys += 0x1000;
        }

        page_directory[pd_idx] = ((uint32_t) page_table_addr) | 3;
        page_table_addr = (uint32_t*)((uint32_t) page_table_addr + 0x1000);
    }
}

void map_page(uint32_t virt, uint32_t phys, uint32_t flags){
    uint32_t pd_idx = virt >> 22;
    uint32_t pt_idx = virt >> 12 & 0x3ff;
    
    ((uint32_t*)page_directory[pd_idx])[pt_idx] = phys | (flags & 0xfff) | 0x01;
}

void initialize_paging(){
    page_directory = (uint32_t*) 0x400000;
    page_table_addr = (uint32_t*)((uint32_t) page_directory + 0x1000);

    for(int i = 0; i < 1024; i++){
        page_directory[i] = 2;
    }
    map_table(0, 0, 3);
    map_table(0x400000, 0x400000, 3);
    for(int i = 0; i < 5; i++){
        map_table(0x30000000 + i*0x400000, 0x30000000 + i*0x400000, 3);
    }

    enable_paging(page_directory);
}