#include "global.h"

pcb_t proc_table[2];
pcb_t *proc_running = &proc_table[0];

gdt_entry_t gdt[8];
gdt_ptr_t gdt_ptr;
tss_t tss;

int8_t reenter = 0;
idt_entry_t idt[256];
idt_ptr_t idt_ptr;
isr_handler_ptr_t isr_handlers[256] = {};