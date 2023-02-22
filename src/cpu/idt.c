#include "../lib-header/idt.h"
#include "../lib-header/isr.h"
#include "../lib-header/portio.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/framebuffer.h"


InterruptService interruptHandlers[IDT_MAX_COUNT];
InterruptDescriptorTable idt;
IDTR idtr = {
    .size = sizeof(idt) - 1,
    .address = &idt
};

void set_idt_gate(uint8_t n, uint32_t input){
    idt.table[n].base_low = (uint16_t) input & 0xffff;

    idt.table[n].selector = SELECTOR;
    idt.table[n].zero = 0;
    idt.table[n].flags = KERNEL_FLAGS;

    idt.table[n].base_high = (uint16_t) ((input >> 16 ) & 0xffff);
}

void init_idt(){
    memset(&idt, 0, sizeof(InterruptDescriptorTable));

    //isr
    set_idt_gate(0, (uint32_t)isr0);
    set_idt_gate(1, (uint32_t)isr1);
    set_idt_gate(2, (uint32_t)isr2);
    set_idt_gate(3, (uint32_t)isr3);
    set_idt_gate(4, (uint32_t)isr4);
    set_idt_gate(5, (uint32_t)isr5);
    set_idt_gate(6, (uint32_t)isr6);
    set_idt_gate(7, (uint32_t)isr7);
    set_idt_gate(8, (uint32_t)isr8);
    set_idt_gate(9, (uint32_t)isr9);
    set_idt_gate(10, (uint32_t)isr10);
    set_idt_gate(11, (uint32_t)isr11);
    set_idt_gate(12, (uint32_t)isr12);
    set_idt_gate(13, (uint32_t)isr13);
    set_idt_gate(14, (uint32_t)isr14);
    set_idt_gate(15, (uint32_t)isr15);
    set_idt_gate(16, (uint32_t)isr16);
    set_idt_gate(17, (uint32_t)isr17);
    set_idt_gate(18, (uint32_t)isr18);
    set_idt_gate(19, (uint32_t)isr19);
    set_idt_gate(20, (uint32_t)isr20);
    set_idt_gate(21, (uint32_t)isr21);
    set_idt_gate(22, (uint32_t)isr22);
    set_idt_gate(23, (uint32_t)isr23);
    set_idt_gate(24, (uint32_t)isr24);
    set_idt_gate(25, (uint32_t)isr25);
    set_idt_gate(26, (uint32_t)isr26);
    set_idt_gate(27, (uint32_t)isr27);
    set_idt_gate(28, (uint32_t)isr28);
    set_idt_gate(29, (uint32_t)isr29);
    set_idt_gate(30, (uint32_t)isr30);
    set_idt_gate(31, (uint32_t)isr31);

    //pic remap
    out(0x20, 0x11);
    out(0xa0, 0x11);
    out(0x21, 0x20);
    out(0xa1, 0x28);
    out(0x21, 0x04);
    out(0xa1, 0x02);
    out(0x21, 0x01);
    out(0xa1, 0x01);
    out(0x21, 0x0);
    out(0xa1, 0x0);


    //irq
    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);

    load_idt(&idtr);
}

char *exception_msg[] = {
    "Division By Zero",                 //0
    "Debug",                            //1
    "Non Maskable Interrupt",           //2
    "Breakpoint",                       //3
    "Into Detected Overflow",           //4
    "Out of Bounds",                    //5
    "Invalid Opcode",                   //6
    "No Coprocessor",                   //7
    "Double Fault",                     //8
    "Coprocessor Segment Overrun",      //9
    "Bad TSS",                          //10
    "Segment Not Present",              //11
    "Stack Fault",                      //12
    "General Protection Fault",         //13
    "Page Fault",                       //14
    "Unknown Interrupt",                //15
    "Coprocessor Fault",                //16
    "Alignment Check",                  //17
    "Machine Check",                    //18
    "Reserved",                         //19
    "Reserved",                         //20
    "Reserved",                         //21
    "Reserved",                         //22
    "Reserved",                         //23
    "Reserved",                         //24
    "Reserved",                         //25
    "Reserved",                         //26
    "Reserved",                         //27
    "Reserved",                         //28
    "Reserved",                         //29
    "Reserved",                         //30
    "Reserved"                          //31
};

void isr_handler(registers r) {
    framebuffer_printDef("received interrupt: 0x");
    char s[3];
    int_toString(r.int_no, s);
    framebuffer_printDef(s);
    framebuffer_printDef("\nexception: ");
    framebuffer_printDef(exception_msg[r.int_no]);
    framebuffer_printDef("\n");
    __asm__ volatile(
        "hlt"
    );
}

void irq_handler(registers r){
    if (r.int_no >=40){
        out(0xa0, 0x20);
    }
    out(0x20, 0x20);

    if(interruptHandlers[r.int_no] != 0){
        InterruptService handler = interruptHandlers[r.int_no];
        handler(r);
    }
}

void register_interrupt_handler(uint8_t n, InterruptService input){
    interruptHandlers[n] = input;
}