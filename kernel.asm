global _start
_start:
    mov byte [gs : 0], 'K'
    mov byte [gs : 2], 'E'
    mov byte [gs : 4], 'R'
    mov byte [gs : 6], 'N'
    mov byte [gs : 8], 'E'
    mov byte [gs : 0xa], 'L'
    jmp $