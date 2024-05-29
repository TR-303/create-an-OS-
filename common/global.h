#pragma once
#include "thread.h"

extern pcb_t proc_table[2];
extern pcb_t *proc_running = &proc_table[0];

#include "gdt.h"

extern gdt_entry_t gdt[8];
extern gdt_ptr_t gdt_ptr;
extern tss_t tss;

#include "int.h"

extern int8_t reenter = 0;
extern idt_entry_t idt[256];
extern idt_ptr_t idt_ptr;
extern isr_handler_ptr_t isr_handlers[256] = {};