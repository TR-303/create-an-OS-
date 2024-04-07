%include "boot.inc"

section mbr vstart=MBR_START_ADDR

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax

    mov sp, 0x7c00

    ;load the loader program from floppy
    mov ah, 0x02
    mov al, 0x04
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, 0x00
    mov bx, LOADER_START_ADDR
    int 0x13

    cmp ah, 0x00
    jne read_failed

    jmp LOADER_START_ADDR

read_failed:
    ;读取失败提示
    
    jmp $

filling:
    times 510-($-$$) db 0
    dw 0xaa55