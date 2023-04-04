
#include "../lib-header/stdtype.h"
#include "../lib-header/gdt.h"
#include "../lib-header/tss.h"


/**
 * global_descriptor_table, predefined GDT.
 * Initial SegmentDescriptor already set properly according to GDT definition in Intel Manual & OSDev.
 * Table entry : [{Null Descriptor}, {Kernel Code}, {Kernel Data (variable, etc)}, ...].
 */

struct GlobalDescriptorTable global_descriptor_table = {
    .table = {
        {0},
        {//kernal data
            .segment_low = 0xffff,
            .base_low = 0,
            .base_mid = 0,  
            .type_bit = 0b1010,
            .non_system = 1,
            .DPL = 0,
            .P = 1,
            .segment_mid = 0xf,
            .AVL = 0,
            .L = 0,
            .DB = 1,   
            .G = 1,
            .base_high = 0,
        },
        {//kernal code
            .segment_low = 0xffff,
            .base_low = 0,
            .base_mid = 0,  
            .type_bit = 0b0010,
            .non_system = 1,
            .DPL = 0,
            .P = 1,
            .segment_mid = 0xf,
            .AVL = 0,
            .L = 0,
            .DB = 1,   
            .G = 1,
            .base_high = 0,
        },
        {//user code
            .segment_low = 0xffff,
            .base_low = 0,
            .base_mid = 0,  
            .type_bit = 0b1010,
            .non_system = 1,
            .DPL = 3,
            .P = 1,
            .segment_mid = 0xf,
            .AVL = 0,
            .L = 0,
            .DB = 1,   
            .G = 1,
            .base_high = 0,
        },
        {//user data
            .segment_low = 0xffff,
            .base_low = 0,
            .base_mid = 0,  
            .type_bit = 0b0010,
            .non_system = 1,
            .DPL = 3,
            .P = 1,
            .segment_mid = 0xf,
            .AVL = 0,
            .L = 0,
            .DB = 1,   
            .G = 1,
            .base_high = 0,
        },
        {//tss
            .segment_low       = sizeof(struct TSSEntry),
            .base_low          = 0,
            .base_mid          = 0,
            .type_bit          = 0x9,
            .non_system        = 0,    // S bit
            .DPL               = 0,    // DPL
            .P                 = 1,    // P bit
            .segment_mid       = (sizeof(struct TSSEntry) & (0xF << 16)) >> 16,
            .AVL               = 0,
            .L                 = 0,    // L bit
            .DB                = 1,    // D/B bit
            .G                 = 0,    // G bit
            .base_high         = 0,
        },
        {0}
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

void gdt_install_tss(void) {
    uint32_t base = (uint32_t) &_interrupt_tss_entry;
    global_descriptor_table.table[5].base_high = (base & (0xFF << 24)) >> 24;
    global_descriptor_table.table[5].base_mid  = (base & (0xFF << 16)) >> 16;
    global_descriptor_table.table[5].base_low  = base & 0xFFFF;
}

