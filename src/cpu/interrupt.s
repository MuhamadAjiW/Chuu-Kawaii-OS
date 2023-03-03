extern main_interrupt_handler
global isr_stub_table
global load_idt
global irq0
global irq1
global irq2

load_idt:
    mov eax, [esp+4]
    lidt [eax]
    ret

%macro interrupt_handler_i 1
isr_stub_%1:
    cli
    push byte 0
    push byte 0d%1
    jmp call_generic_handler
%endmacro

interrupt_handler_i 0
interrupt_handler_i 1
interrupt_handler_i 2
interrupt_handler_i 3
interrupt_handler_i 4
interrupt_handler_i 5
interrupt_handler_i 6
interrupt_handler_i 7
interrupt_handler_i 8
interrupt_handler_i 9
interrupt_handler_i 10
interrupt_handler_i 11
interrupt_handler_i 12
interrupt_handler_i 13
interrupt_handler_i 14
interrupt_handler_i 15
interrupt_handler_i 16
interrupt_handler_i 17
interrupt_handler_i 18
interrupt_handler_i 19
interrupt_handler_i 20
interrupt_handler_i 21
interrupt_handler_i 22
interrupt_handler_i 23
interrupt_handler_i 24
interrupt_handler_i 25
interrupt_handler_i 26
interrupt_handler_i 27
interrupt_handler_i 28
interrupt_handler_i 29
interrupt_handler_i 30
interrupt_handler_i 31

;irqs
interrupt_handler_i 32
interrupt_handler_i 33
interrupt_handler_i 34

call_generic_handler:
    pusha
	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
    
    call main_interrupt_handler

    pop eax 
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	add esp, 8
	sti

    iret

isr_stub_table:
%assign i 0
%rep 35
    dd isr_stub_%+i
%assign i i+1
%endrep

;-----------------------
