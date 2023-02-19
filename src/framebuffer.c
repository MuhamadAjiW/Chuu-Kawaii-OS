#include "lib-header/framebuffer.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/portio.h"

void framebuffer_set_cursor(uint8_t r, uint8_t c) {
    uint16_t position;
    
    position = r * 80 + c;
    out(CURSOR_PORT_CMD, 0x0F);
    out(CURSOR_PORT_DATA, (uint8_t) position);
    out(CURSOR_PORT_CMD, 0x0E);
    out(CURSOR_PORT_DATA, (uint8_t) position >> 8);
}

void framebuffer_write(uint8_t row, uint8_t col, char c, uint8_t fg, uint8_t bg) {
    uint8_t * location;
    uint8_t color;

    location = (uint8_t*) MEMORY_FRAMEBUFFER + (row* 80 + col)*2;
    color = (bg << 4) | (fg & 0x0f);

    memset(location, c, 1);
    memset(location+1, color, 1);
}

void framebuffer_clear(void) {
    for (int row = 0; row < 25; row++){
        for (int col = 0; col < 80; col++){
            framebuffer_write(row, col, 0, 0x0, 0xf);
        }
    }
}
