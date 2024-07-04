#pragma once
#include "common.h"
extern uint32_t ticks;

#include "thread.h"

extern tcb_t *proc_table[10];
extern tcb_t *proc_running;
extern int proc_idx;

#include "gdt.h"

extern gdt_entry_t gdt[8];
extern gdt_ptr_t gdt_ptr;
extern tss_t tss;

#include "int.h"

extern idt_entry_t idt[256];
extern idt_ptr_t idt_ptr;
extern isr_handler_ptr_t isr_handlers[256];

#include "syscall.h"

extern syscall_ptr_t syscalls[10];