extern isr_handler
extern current_proc
extern proc_table
global resume

reenter db 0

%macro ISR 1
    global isr%1
    isr%1:
    ;     cmp byte [reenter], 0
    ;     jz .permit%1
    ;     add esp, 4
    ;     iret
    ; .permit%1:
    ;     inc byte [reenter]
    ;     sti
        push %1
        jmp common_handler
%endmacro

%macro ISR_NOERRCODE 1
    global isr%1
    isr%1:
        ; push eax
        ; mov al, 0x20
        ; out 0x20, al
        ; pop eax
    ;     cmp byte [reenter], 0
    ;     jz .permit%1
    ;     iret
    ; .permit%1:
    ;     inc byte [reenter]
    ;     sti
        push 0xffffffff
        push %1
        jmp common_handler
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR 8
ISR_NOERRCODE 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR 30
ISR_NOERRCODE 31

ISR_NOERRCODE 32
ISR_NOERRCODE 33
ISR_NOERRCODE 34
ISR_NOERRCODE 35
ISR_NOERRCODE 36
ISR_NOERRCODE 37
ISR_NOERRCODE 38
ISR_NOERRCODE 39
ISR_NOERRCODE 40
ISR_NOERRCODE 41
ISR_NOERRCODE 42
ISR_NOERRCODE 43
ISR_NOERRCODE 44
ISR_NOERRCODE 45
ISR_NOERRCODE 46
ISR_NOERRCODE 47

common_handler:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax

    mov ebx, [current_proc]
    shl ebx, 2
    mov eax, proc_table[ebx]
    mov [eax + 76], esp

    call isr_handler
    dec byte [reenter]

resume:
    mov ebx, [current_proc]
    shl ebx, 2
    mov eax, proc_table[ebx]
    mov esp, [eax + 76]
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8  ;skip errcode and intnum
    iret