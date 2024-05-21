global resume
extern proc_table
extern tss

[BITS 32]
resume:
    mov esp, proc_table

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 4
    iret