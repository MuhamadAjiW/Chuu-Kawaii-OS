#include "lib-header/framebuffer.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/portio.h"

void framebuffer_set_cursor(uint8_t r, uint8_t c) {
    uint16_t position;
    
    position = r * 80 + c;
    out(0x3D4, 0x0F);
    out(0x3D5, (uint8_t) (position & 0xff));
    out(0x3D4, 0x0E);
    out(0x3D5, (uint8_t) ((position >> 8) & 0xff));
}

void framebuffer_write(uint8_t row, uint8_t col, char c, uint8_t fg, uint8_t bg) {
    uint16_t color;
    volatile uint16_t * location;

    color = (bg << 4) | (fg & 0x0F);
    location = (volatile uint16_t*) 0xb8000 + (row* 80 + col);
    *location = (color << 8) | c;
}

void framebuffer_clear(void) {
    for (int row = 0; row < 25; row++){
        for (int col = 0; col < 80; col++){
            framebuffer_write(row, col, ' ', 0x0, 0xf);
        }
    }
}
