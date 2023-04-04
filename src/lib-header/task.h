#ifndef _TASK_H
#define _TASK_H

#include "stdtype.h"

#define PSTATE_ALIVE 0
#define PSTATE_DONE 1
#define PSTATE_DEAD 2

//work in progress
struct Task
{
    char* name;
    
    //registers
    uint32_t pid; //id
    uint32_t esp; //stack bottom
    uint32_t ebp; //stack top
    uint32_t eip; //next instruction
    uint32_t cr3; //virtual address space
    uint32_t state; //state

    //Auxillary
    struct Task* next; //next process

};
typedef struct Task Task;

Task* createProcess(char* processName, void* addr);
void initialize_tasking();

#endif