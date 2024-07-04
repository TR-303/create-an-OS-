%include "boot.inc"

section loader vstart=LOADER_START_ADDR

loader_entry:
    jmp loader_start

gdt_start:
            Descriptor  0                   ,0          ,0
LdCode32:   Descriptor  LDCODE32_START      ,0xfffff    ,ATTR_LDCODE32
Data:       Descriptor  DATA_START          ,0xfffff    ,ATTR_DATA
Vedio:      Descriptor  VEDIO_START         ,0x7fff     ,ATTR_VIDEO

gdt_ptr:
    dw $-gdt_start-1
    dd gdt_start

SelectorLdCode32    equ (1<<3) | 0b000
SelectorData        equ (2<<3) | 0b000
SelectorVideo       equ (3<<3) | 0b000

kernel_elf_file_name    db 'KERNEL  '

[BITS 16]
loader_start:

    ;read kernel.elf
    mov ax, KERNEL_ELF_PHI_START / 0x10
    mov es, ax
    push KERNEL_ELF_PHI_START % 0x10
    push kernel_elf_file_name
    call [READ_FAT12_FILE_FUNC_ADDR_ADDR]
    add sp, 4

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
    mov ax, SelectorData
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax

    ; set video segment
    mov ax, SelectorVideo
    mov gs, ax

    call turnon_paging
    
    call load_kernel

    ;jump into kernel!
    jmp KERNEL_VIR_START


;create last 256*1024 ptes at PAGE_TABLE_START
turnon_paging:

    ;set pde[768] pointing to first pte
    mov eax, KERNEL_PTE_START | 0b111
    mov dword [KERNEL_PDE_START + 768 * 4], eax
    mov dword [KERNEL_PDE_START + 0 * 4], eax

    ;create first 256 pte pointing to 0-0x100000
    mov eax, 0 | 0b111
    mov ebx, KERNEL_PTE_START
    mov ecx, 256
.lp_create_first_ptes:
    mov dword [ebx], eax
    add ebx, 4
    add eax, PAGE_SIZE
    loop .lp_create_first_ptes

    ;turn on paging mode
    
    sgdt [gdt_ptr]

    ;mov vedio segment to > 0xc0000000
    or byte [gdt_start + (3 << 3) + 7], 0xc0

    ;load gdt again
    add dword [gdt_ptr + 2], 0xc0000000
    lgdt [gdt_ptr]

    ;set cr3
    mov eax, KERNEL_PDE_START
    mov cr3, eax

    ;set cr0 bit
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

;load kernel program from elf
load_kernel:

    ;set page for kernel
    mov dword [KERNEL_PDE_START + 769 * 4], (KERNEL_PTE_START + PAGE_SIZE) | 0b111
    mov eax, KERNEL_PHY_START | 0b111
    mov ebx, KERNEL_PTE_START + PAGE_SIZE
    mov ecx, 512
.lp_create_first_ptes:
    mov dword [ebx], eax
    add ebx, 4
    add eax, PAGE_SIZE
    loop .lp_create_first_ptes

    ;copy code from elf
    mov ecx, 0
    mov cx, [KERNEL_ELF_PHI_START + 0x2c]
    mov ebx, [KERNEL_ELF_PHI_START + 0x1c]
    add ebx, KERNEL_ELF_PHI_START

.load_kernel_lp_copy_segment:
    push ecx
    mov esi, [ebx + 4]
    add esi, KERNEL_ELF_PHI_START
    mov edi, [ebx + 8]
    mov ecx, [ebx + 0x14]
    rep movsb
    pop ecx
    add ebx, 0x20
    loop .load_kernel_lp_copy_segment

    ret