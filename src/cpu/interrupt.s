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

%macro no_error_code_interrupt_handler 1
isr_stub_%1:
    cli
    push dword 0
    push dword 0d%1
    jmp call_generic_handler
%endmacro

%macro error_code_interrupt_handler 1
isr_stub_%1:
    cli
    push dword 0d%1
    jmp call_generic_handler
%endmacro

no_error_code_interrupt_handler 0  ; 0x0  - Division by zero
no_error_code_interrupt_handler 1  ; 0x1  - Debug Exception
no_error_code_interrupt_handler 2  ; 0x2  - NMI, Non-Maskable Interrupt
no_error_code_interrupt_handler 3  ; 0x3  - Breakpoint Exception
no_error_code_interrupt_handler 4  ; 0x4  - INTO Overflow
no_error_code_interrupt_handler 5  ; 0x5  - Out of Bounds
no_error_code_interrupt_handler 6  ; 0x6  - Invalid Opcode
no_error_code_interrupt_handler 7  ; 0x7  - Device Not Available
error_code_interrupt_handler    8  ; 0x8  - Double Fault
no_error_code_interrupt_handler 9  ; 0x9  - Deprecated
error_code_interrupt_handler    10 ; 0xA  - Invalid TSS
error_code_interrupt_handler    11 ; 0xB  - Segment Not Present
error_code_interrupt_handler    12 ; 0xC  - Stack-Segment Fault
error_code_interrupt_handler    13 ; 0xD  - General Protection Fault
error_code_interrupt_handler    14 ; 0xE  - Page Fault
no_error_code_interrupt_handler 15 ; 0xF  - Reserved
no_error_code_interrupt_handler 16 ; 0x10 - x87 Floating-Point Exception
error_code_interrupt_handler    17 ; 0x11 - Alignment Check Exception
no_error_code_interrupt_handler 18 ; 0x12 - Machine Check Exception
no_error_code_interrupt_handler 19 ; 0x13 - SIMD Floating-Point Exception
no_error_code_interrupt_handler 20 ; 0x14 - Virtualization Exception
no_error_code_interrupt_handler 21 ; 0x15 - Control Protection Exception
no_error_code_interrupt_handler 22 ; 0x16 - Reserved
no_error_code_interrupt_handler 23 ; 0x17 - Reserved
no_error_code_interrupt_handler 24 ; 0x18 - Reserved
no_error_code_interrupt_handler 25 ; 0x19 - Reserved
no_error_code_interrupt_handler 26 ; 0x1A - Reserved
no_error_code_interrupt_handler 27 ; 0x1B - Reserved
no_error_code_interrupt_handler 28 ; 0x1C - Hypervisor Injection Exception
no_error_code_interrupt_handler 29 ; 0x1D - VMM Communication Exception
error_code_interrupt_handler    30 ; 0x1E - Security Exception
no_error_code_interrupt_handler 31 ; 0x1F - Reserved

;IRQ & syscall
%assign i 32 
%rep    16
no_error_code_interrupt_handler i
%assign i i+1 
%endrep

isr_stub_48:
    cli
    push dword 48
    jmp call_syscall_handler

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

call_syscall_handler:
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
	add esp, 0x4
	sti

    iret

isr_stub_table:
%assign i 0
%rep 49
    dd isr_stub_%+i
%assign i i+1
%endrep

;-----------------------
