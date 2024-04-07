%include "boot.inc"

section loader vstart=LOADER_START_ADDR

loader_entry:
    jmp loader_start

gdt_start:
    Descriptor  0                       ,0                      ,0
    Descriptor  CODE32_START            ,CODE32_LIMIT           ,CODE32_ATTR
    Descriptor  DATA_ATTR               ,DATA_LIMIT             ,DATA_ATTR
    Descriptor  VIDEO_START             ,VIDEO_LIMIT            ,VIDEO_ATTR

gdt_ptr:
    dw $-gdt_start-1
    dd gdt_start

SelectorCode32  equ (1<<3) | 0b000
SelectorData    equ (2<<3) | 0b000
SelectorVideo   equ (3<<3) | 0b000

[BITS 16]
loader_start:

    ;disable interupt
    cli

    ;load gdt
    lgdt [gdt_ptr]

    ;enable A20
    in al, 0x92
    or al, 0b00000010
    out 0x92, al

    ;set cr0 bit
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ;jump into protection mode!
    jmp dword SelectorCode32:code32entry

[BITS 32]
code32entry:

    ;set data segment
    mov ax, SelectorData
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; set video segment
    mov ax, SelectorVideo
    mov gs, ax

    ;validation
    mov byte [gs:0], 'P'
    mov byte [gs:1], 0x07

    jmp $