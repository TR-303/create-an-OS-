#include "global.h"

tcb_t *proc_table[10] = {0};
tcb_t *proc_running;
int proc_idx = 0;

uint32_t ticks = 0;

gdt_entry_t gdt[8] = {0};
gdt_ptr_t gdt_ptr;
tss_t tss;

idt_entry_t idt[256] = {0};
idt_ptr_t idt_ptr;
isr_handler_ptr_t isr_handlers[256] = {0};

syscall_ptr_t syscalls[10] = {0};