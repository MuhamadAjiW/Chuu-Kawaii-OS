#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#define MISC_OUT_REG 0x3c2
#define FEAT_CONT_REG 0x3ca

#define ATTR_ADDR_REG 0x3c0
#define ATTR_DATA_REG 0x3c1

#define CRT_ADDR_REG 0x3d4
#define CRT_DATA_REG 0x3d5

#define MEM_MODE_REG 0x3c4

#define DAC_MASK_REG 0x3c6
#define DAC_READ_REG 0x3c7
#define DAC_WRITE_REG 0x3c8
#define DAC_DATA_REG 0x3c9
#define DAC_STATE_REG 0x3c7

#define GRAP_ADDR_REG 0x3ce
#define GRAP_DATA_REG 0x3cf

#define INPUT_STATUS_1 0x3da

void initialize_vga();

#endif