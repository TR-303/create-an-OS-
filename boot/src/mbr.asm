[BITS 16]

%include "boot.inc"

section mbr vstart=MBR_START_ADDR

%include "fat12hdr.inc"

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax

    mov ax, 0xb800
    mov gs, ax

    ;init stack
    mov sp, 0x7c00

    push LOADER_START_ADDR
    push loader_file_name
    call read_fat12_file
    add sp, 4
    cmp ax, 1
    jz .read_loader_success

    ;loader not found
    jmp $

.read_loader_success:

    mov word [READ_FAT12_FILE_FUNC_ADDR_ADDR], read_fat12_file

    ;jmp into loader
    jmp LOADER_START_ADDR

;read sectors from floppy
;logical sector is 0-index
;arg1: start sector
;arg2: sector count
;es : arg3: target addr
read_floppy:
    
    push bp
    mov bp, sp
    push ax
    push bx
    push cx
    push dx

    mov ax, [bp + 4]

    mov dx, 18
    div dl
    mov dh, al
    and dh, 0b1
    mov ch, al
    shr ch, 1
    mov al, [bp + 6]
    mov cl, ah
    inc cl
    mov dl, 0
    mov bx, [bp + 8]
.read_floppy_try_again:
    mov ah, 0x02
    int 0x13

    cmp ah, 0
    jz .read_floppy_ok
    mov byte [gs : 0], '1'
    jmp .read_floppy_try_again

.read_floppy_ok:

    pop dx
    pop cx
    pop bx
    pop ax
    pop bp
    ret

;read file from fat12 floppy
;ds : arg1: 8 byte file name
;es : arg2: target addr
;ret ax: success or not
read_fat12_file:

    ;preseve 2 sectors as buff
    READ_FILE_BUFFER_START equ 0x7000

    push bp
    mov bp, sp
    push ax
    push bx
    push di
    push cx
    
    mov word [next_root_dir_sector], 19

.read_fat12_file_lp_read_root_dir_sectors:

    cmp word [next_root_dir_sector], 33
    jnz .read_fat12_file_read_root_dir_sectors_go_on

    ;file not found
    mov word [bp - 2], 0
    jmp .read_fat12_file_end

.read_fat12_file_read_root_dir_sectors_go_on:

    push es
    mov ax, ds
    mov es, ax
    push READ_FILE_BUFFER_START
    push 1
    push word [next_root_dir_sector]
    call read_floppy
    add sp, 6
    inc word [next_root_dir_sector]
    pop es

    mov bx, READ_FILE_BUFFER_START - 0x20

.read_fat12_file_lp_find_file:
    and bx, 0xffe0
    add bx, 0x20
    cmp bx, READ_FILE_BUFFER_START + 0x200
    jz .read_fat12_file_lp_read_root_dir_sectors

    mov di, [bp + 4]
    mov cx, 8

.read_fat12_file_lp_cmp_str:

    mov al, [bx]
    cmp al, [di]
    jnz .read_fat12_file_lp_find_file
    inc bx
    inc di
    loop .read_fat12_file_lp_cmp_str
    
.read_fat12_file_load_file:

    mov ax, [bx + 18]

.read_fat12_file_lp_load_file_sectors:

    push word [bp + 6]
    push 1
    mov cx, ax
    add cx, 31
    push cx
    call read_floppy
    add sp, 6
    add word [bp + 6], 0x200

    mov cx, ax
    and cx, 0b1
    mov [is_odd], cl
    shr ax, 1
    mov cl, 3
    mul cl
    mov bx, ax
    and bx, 0x1ff
    cmp byte [is_odd], 1
    jnz .read_fat12_file_clus_is_even_1
    inc bx
.read_fat12_file_clus_is_even_1:
    shr ax, 9
    inc ax
    
    push es
    mov dx, ds
    mov es, dx
    push READ_FILE_BUFFER_START
    push 2
    push ax
    call read_floppy
    add sp, 6
    pop es

    mov ax, [bx + READ_FILE_BUFFER_START]
    cmp byte [is_odd], 1
    jnz .read_fat12_file_clus_is_even_2
    shr ax, 4
.read_fat12_file_clus_is_even_2:
    and ax, 0xfff
    
    cmp ax, 0xff8
    jnl .read_fat12_file_done
    jmp .read_fat12_file_lp_load_file_sectors
    
.read_fat12_file_done:

    mov word [bp - 2], 1

.read_fat12_file_end:
    pop cx
    pop di
    pop bx
    pop ax
    pop bp
    ret

.read_fat12_file_vars:

    next_root_dir_sector    dw 19
    is_odd                  db 0

loader_file_name    db 'LOADER  '

filling:
    times 510-($-$$) db 0
    dw 0xaa55