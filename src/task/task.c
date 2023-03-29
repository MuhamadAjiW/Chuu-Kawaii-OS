#include "../lib-header/task.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/memory_manager.h"

Task* current = 0;
uint32_t process_counter = 0;
uint8_t enabled = 0;

Task* createProcess(char* processName, void* addr){
    Task* process = (Task*) malloc(sizeof(Task));

    memset(process, 0, sizeof(Task));

    process->name = processName;
    process->pid = ++process_counter;
    process->eip = (uint32_t) addr;
    process->state = PSTATE_ALIVE;
    process->esp = (uint32_t) malloc (4096);

    __asm__ volatile ("mov %%cr3, %%eax" : "=a" (process->cr3));

    uint32_t* stack = (uint32_t*) (process->esp + 4096);

    process->ebp = process->esp;

    stack -= 1;
    *stack = 0x202; //eflags
    stack -= 1;
    *stack = 0x8; //cs
    stack -= 1;
    *stack = (uint32_t) addr; //eip
    stack -= 1;
    *stack = 0; //eax
    stack -= 1;
    *stack = 0; //ebx
    stack -= 1;
    *stack = 0; //ecx
    stack -= 1;
    *stack = 0; //edx
    stack -= 1;
    *stack = 0; //esi
    stack -= 1;
    *stack = 0; //edi
    stack -= 1;
    *stack = process->esp + 4096; //stack
    stack -= 1;
    *stack = 0x10; //ds
    stack -= 1;
    *stack = 0x10; //fs
    stack -= 1;
    *stack = 0x10; //es
    stack -= 1;
    *stack = 0x10; //gs
    
    process->esp = (uint32_t) stack;

    return process;
}

void idle_process(){

}

void initialize_tasking(){
    enabled = 1;
    current = createProcess("idle", idle_process);

    return;
}
