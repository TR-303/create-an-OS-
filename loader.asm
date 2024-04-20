%include "boot.inc"

section loader vstart=LOADER_START_ADDR

loader_entry:
    jmp loader_start

gdt_start:
            Descriptor  0                   ,0          ,0
LdCode32:   Descriptor  LDCODE32_START      ,LIMIT_1MB  ,ATTR_EXEC
LdData:     Descriptor  LDDATA_START        ,LIMIT_1MB  ,ATTR_DATA
Page:       Descriptor  PAGE_TABLE_START    ,LIMIT_1MB  ,ATTR_DATA
Vedio:      Descriptor  VEDIO_START         ,0x7fff     ,ATTR_DATA

Kernel:     Descriptor  KERNEL_VIR_START    ,LIMIT_1MB  ,ATTR_DATA

gdt_ptr:
    dw $-gdt_start-1
    dd gdt_start

SelectorLdCode32    equ (1<<3) | 0b000
SelectorLdData      equ (2<<3) | 0b000
SelectorPage        equ (3<<3) | 0b000
SelectorVideo       equ (4<<3) | 0b000

SelectorKernel      equ (5<<3) | 0b000

kernel_elf_file_name    db 'KERNEL  '

[BITS 16]
loader_start:

    ;read kernel.elf
    mov ax, KERNEL_ELF_PHI_START / 0x10
    mov es, ax
    push KERNEL_ELF_PHI_START % 0x10
    push kernel_elf_file_name
    call [READ_FAT12_FILE_FUNC_ADDR_ADDR]
    add sp, 8

    cmp ax, 1
    jz .read_kernel_elf_success

    ;kernel not found
    jmp $

.read_kernel_elf_success:

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
    jmp dword SelectorLdCode32:code32entry

[BITS 32]
code32entry:

    ;set data and stack segment
    mov ax, SelectorLdData
    mov ds, ax
    mov es, ax
    mov ss, ax

    ;set page segment
    mov ax, SelectorPage
    mov es, ax

    ; set video segment
    mov ax, SelectorVideo
    mov gs, ax

    call turnon_paging

    call load_kernel

    ;jmp into kernel!
    jmp SelectorKernel:0

;create last 256*1024 ptes at PAGE_TABLE_START
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

    ;create first 1024 pte pointing to 0-0x100000
    push 0xc0000000
    push 0
    call set_page
    add esp, 8

    ;turn on paging mode
    
    sgdt [gdt_ptr]

    ;mov vedio segment to > 0xc0000000
    or byte [gdt_start + (4 << 3) + 7], 0xc0

    ;mov lower 1MB segment to > 0xc0000000
    or byte [gdt_start + (2 << 3) + 7], 0xc0

    ;mov page segment to > 0xc0000000
    mov word [gdt_start + (3 << 3) + 2], PAGE_TABLE_VIR_START & 0xffff
    mov byte [gdt_start + (3 << 3) + 4], (PAGE_TABLE_VIR_START >> 16) & 0xff
    mov byte [gdt_start + (3 << 3) + 7], PAGE_TABLE_VIR_START >> 24

    ;load gdt again
    add dword [gdt_ptr + 2], 0xc0000000
    lgdt [gdt_ptr]

    ;set cr3
    mov eax, PAGE_TABLE_START + PAGE_DIR_PHY_ADDR
    mov cr3, eax

    ;set cr0 bit
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ;paging is now on
    ret

;setup 1024 ptes(a page)
;arg1: physical addr
;arg2: virtual addr
set_page:
    push ebp
    mov ebp, esp
    push eax
    push ebx
    push ecx
    
    mov eax, [ebp + 8]      ;physical addr
    mov ebx, [ebp + 12]     ;virtual addr
    shr ebx, 22
    sub ebx, 768
    shl ebx, 12
    or eax, 0b111
    mov ecx, 1024
.lp_set_page:
    mov [es:ebx], eax
    add eax, PAGE_SIZE
    add ebx, 4
    loop .lp_set_page

    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret

;load kernel program from elf
load_kernel:

    ;set page for kernel
    push KERNEL_VIR_START
    push KERNEL_PHI_START
    call set_page
    add sp, 8

    ;copy code from elf
    mov ebx, [KERNEL_ELF_PHI_START + 0x1c]
    mov ecx, [KERNEL_ELF_PHI_START + ebx + 0x10]
    mov esi, [KERNEL_ELF_PHI_START + ebx + 0x4]
    add esi, KERNEL_ELF_PHI_START
    mov ax, SelectorKernel
    mov es, ax
    mov edi, 0

    rep movsb

    ;turn kernel into exec segment
    and word [gdt_start + (5 << 3) + 5], 0xf0ff
    or word [gdt_start + (5 << 3) + 5], ATTR_EXEC

    ret