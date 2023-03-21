
#include "../lib-header/stdtype.h"
#include "../lib-header/gdt.h"

/**
 * global_descriptor_table, predefined GDT.
 * Initial SegmentDescriptor already set properly according to GDT definition in Intel Manual & OSDev.
 * Table entry : [{Null Descriptor}, {Kernel Code}, {Kernel Data (variable, etc)}, ...].
 */

struct GlobalDescriptorTable global_descriptor_table = {
    .table = {
        {0},
        {
            .segment_low = 0xff,
            .base_low = 0,
            .base_mid = 0,  
            .type_bit = 0b1010,
            .non_system = 1,
            .DPL = 0,
            .P = 1,
            .segment_mid = 0b1111,
            .AVL = 0,
            .L = 0,
            .DB = 1,   
            .G = 1,
            .base_high = 0,
        },
        {
            .segment_low = 0xff,
            .base_low = 0,
            .base_mid = 0,  
            .type_bit = 0b0010,
            .non_system = 1,
            .DPL = 0,
            .P = 1,
            .segment_mid = 0b1111,
            .AVL = 0,
            .L = 0,
            .DB = 1,   
            .G = 1,
            .base_high = 0,
        }
    }
};

/**
 * _gdt_gdtr, predefined system GDTR. 
 * GDT pointed by this variable is already set to point global_descriptor_table above.
 * From: https://wiki.osdev.org/Global_Descriptor_Table, GDTR.size is GDT size minus 1.
 */
struct GDTR _gdt_gdtr = {
    .size = sizeof(global_descriptor_table),
    .address = &global_descriptor_table
};
