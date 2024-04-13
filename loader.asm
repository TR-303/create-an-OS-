%include "boot.inc"

section loader vstart=LOADER_START_ADDR

loader_entry:
    jmp loader_start

gdt_start:
    Descriptor  0           ,0          ,0
    Descriptor  0           ,0xfffff    ,ATTR_EXEC
    Descriptor  0           ,0xfffff    ,ATTR_DATA
    Descriptor  0x100000    ,0xfffff    ,ATTR_DATA
    Descriptor  0xb8000     ,0x7fff     ,ATTR_DATA
    Descriptor  0xc010000   ,0xfffff    ,ATTR_DATA

gdt_ptr:
    dw $-gdt_start-1
    dd gdt_start

SelectorCode32  equ (1<<3) | 0b000
SelectorData    equ (2<<3) | 0b000
SelectorPage    equ (3<<3) | 0b000
SelectorVideo   equ (4<<3) | 0b000
SelectorKeElf   equ (5<<3) | 0b000

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
    mov ss, ax

    ;set page segment
    mov ax, SelectorPage
    mov es, ax

    ; set video segment
    mov ax, SelectorVideo
    mov gs, ax

    call turnon_paging

    ;validation
    mov byte [gs:0], 'P'
    mov byte [gs:1], 0x07

    jmp $

turnon_paging:

    ;create pdes
    mov eax, 0x100000 | 0b111
    mov ebx, PAGE_DIR_PHY_ADDR + 768 * 4
    mov ecx, 256
.lp_create_pdes:
    mov [es:ebx], eax
    add eax, PAGE_TABLE_SIZE
    add ebx, 4
    loop .lp_create_pdes

    ;set pde[1023] pointing to itself
    mov eax, PAGE_DIR_PHY_ADDR | 0b111
    mov [es:PAGE_DIR_PHY_ADDR + 1023 * 4], eax

    ;set pde[0] and pde[768] pointing to first pte
    mov eax, 0x100000 | 0b111
    mov [es:PAGE_DIR_PHY_ADDR + 0 * 4], eax
    mov [es:PAGE_DIR_PHY_ADDR + 768 * 4], eax

    ;create first pte pointing to 0-0x100000
    mov eax, 0 | 0b111
    mov ebx, 0
    mov ecx, 256
.lp_create_first_pte:
    mov [es:ebx], eax
    add eax, PAGE_SIZE
    add ebx, 4
    loop .lp_create_first_pte

    ;turn on paging mode
    
    sgdt [gdt_ptr]

    ;moving vedio segment to > 0xc0000000
    or byte [gdt_start + (4 << 3) + 7], 0xc0

    ;moving page segment to > 0xc0000000
    mov word [gdt_start + (3 << 3) + 2], PAGE_TABLE_VIR_START & 0xffff
    mov byte [gdt_start + (3 << 3) + 4], (PAGE_TABLE_VIR_START >> 16) & 0xff
    mov byte [gdt_start + (3 << 3) + 7], PAGE_TABLE_VIR_START >> 24

    add dword [gdt_ptr + 2], 0xc0000000

    ;set cr3
    mov eax, PAGE_START + PAGE_DIR_PHY_ADDR
    mov cr3, eax

    ;set cr0 bit
    mov eax, cr0
    or eax, (1 << 31)
    mov cr0, eax

    ;load gdt again
    lgdt [gdt_ptr]

    ret