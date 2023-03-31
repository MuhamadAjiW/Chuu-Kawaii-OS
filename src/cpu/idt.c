#include "../lib-header/idt.h"
#include "../lib-header/isr.h"
#include "../lib-header/tss.h"
#include "../lib-header/portio.h"
#include "../lib-header/stdmem.h"
#include "../lib-header/string.h"
#include "../lib-header/graphics.h"

extern void* isr_stub_table[];

struct TSSEntry _interrupt_tss_entry;
InterruptService interruptHandlers[IDT_MAX_COUNT];
InterruptDescriptorTable idt;
IDTR idtr = {
    .size = sizeof(idt) - 1,
    .address = &idt
};

void set_idt_gate(uint8_t n, void* input){
    idt.table[n].base_low = (uint32_t) input;

    idt.table[n].selector = SELECTOR;
    idt.table[n].zero = 0;
    idt.table[n].flags = KERNEL_FLAGS;

    idt.table[n].base_high = (uint32_t)input >> 16 ;
}

void initialize_idt(){
    memset(&idt, 0, sizeof(InterruptDescriptorTable));

    //isr
    for(uint8_t i = 0; i < 48; i++){
        set_idt_gate(i, isr_stub_table[i]);
    }

    load_idt(&idtr);
    
    __asm__ volatile ("sti");
}

void pic_remap(){

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
    "SIMD Floating-Point Exception",    //19
    "Virtualization Exception",         //20
    "Control Protection Exception",     //21
    "Intel Reserved",                   //22
    "Intel Reserved",                   //23
    "Intel Reserved",                   //24
    "Intel Reserved",                   //25
    "Intel Reserved",                   //26
    "Intel Reserved",                   //27
    "Hypervisor Injection Exception",   //28
    "VMM Communication Exception",      //29
    "Security Exception",               //30
    "Intel Reserved"                    //31
};

void page_fault_handler(){
    __asm__ volatile ("iret");
}

void main_interrupt_handler(registers r){
    //if(r.int_no == 14){
    //    page_fault_handler();
    //}
    if(r.int_no < 32){
        graphics_print("received interrupt: 0x");
        char s[3];
        int_toString(r.int_no, s);
        graphics_print(s);
        graphics_print("\nexception: ");
        graphics_print(exception_msg[r.int_no]);
        graphics_print("\nerror code: ");
        int_toString(r.err_code, s);
        graphics_print(s);
        __asm__ volatile("hlt");
    }
    else{
        if (r.int_no >=40){
            out(0xa0, 0x20);
        }

        out(0x20, 0x20);

        if(interruptHandlers[r.int_no] != 0){
            InterruptService handler = interruptHandlers[r.int_no];
            handler(r);
        }
    }
}

void register_interrupt_handler(uint8_t n, InterruptService input){
    interruptHandlers[n] = input;
}

void set_tss_kernel_current_stack(void) {
    uint32_t stack_ptr;
    // Reading base stack frame instead esp
    __asm__ volatile ("mov %%ebp, %0": "=r"(stack_ptr) : /* <Empty> */);
    // Add 8 because 4 for ret address and other 4 is for stack_ptr variable
    _interrupt_tss_entry.esp0 = stack_ptr + 8; 
}
