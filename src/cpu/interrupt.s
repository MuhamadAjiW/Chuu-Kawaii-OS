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


%assign i 0
%rep 128
    interrupt_handler_i i
%assign i i+1
%endrep


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
%rep 128
    dd isr_stub_%+i
%assign i i+1
%endrep

;-----------------------
