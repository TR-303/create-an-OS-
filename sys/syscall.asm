extern syscall_handler
extern current_proc
extern proc_table

global syscall
syscall:
    push 0
    push 0
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax

    call syscall_handler

global syscall_resume
syscall_resume:
    pop gs
    pop fs
    pop es
    pop ds
    pop edi
    pop esi
    pop ebp
    add esp, 4  ;ignore esp
    pop ebx
    pop edx
    pop ecx
    add esp, 12 ;ignore eax 0 0, eax is the return
    iret

global exec_start
exec_start:
    mov ebx, [current_proc]
    shl ebx, 2
    mov edx, proc_table[ebx]
    mov esp, [edx + 76]
    jmp syscall_resume