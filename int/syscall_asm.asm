extern syscall_handler

global syscall
syscall:
    push 0
    push 0
    pusha
    push ds
    push es
    push fs
    push gs

    call syscall_handler

    pop gs
    pop fs
    pop es
    pop ds
    pop ebp
    pop edi
    pop esi
    add esp, 4
    pop edx
    pop ecx
    pop ebx
    add esp, 12
    iret

global get_ticks
get_ticks:
    mov eax, 0
    int 0x80
    ret
