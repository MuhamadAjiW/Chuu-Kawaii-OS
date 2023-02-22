#include "timer.h"
#include "isr.h"
#include "../lib-header/portio.h"
#include "../lib-header/framebuffer.h"

uint32_t tick = 0;
char buffer[256];

static void timer_callback(){
    tick++;
    //framebuffer_printDef("Tick: ");

    int_toString(tick, buffer);
    
    //framebuffer_printDef(buffer);
    //framebuffer_printDef("\n");    
}

void init_timer(uint32_t freq){
    register_interrupt_handler(32, timer_callback);

    uint32_t x = 1193182 / freq;
    uint8_t low = (uint8_t) (x & 0xff);
    uint8_t high = (uint8_t) ( (x >> 8) & 0xff);

    out(0x43, 0x36);
    out(0x40, low);
    out(0x40, high);
}