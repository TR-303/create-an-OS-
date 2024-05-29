#pragma once
#include "common.h"

extern int8_t reenter;

struct idt_entry
{
    uint16_t addr_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t attributes;
    uint16_t addr_high;
} __attribute__((packed));
typedef struct idt_entry idt_entry_t;

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute((packed));
typedef struct idt_ptr idt_ptr_t;

struct interrupt_stack_frame
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, _esp, ebx, edx, ecx, eax;
    uint32_t num;
    uint32_t err_code;
    uint32_t eip, cs, eflags, esp, ss;
} __attribute((packed));

typedef struct interrupt_stack_frame isr_param_t;

typedef void (*isr_handler_ptr_t)(isr_param_t);

void enable_interrupt();

void diable_interrupt();

void isr_handler();

void set_igt_entry(int num, uint32_t addr, uint16_t selector, uint8_t attributes);

void init_interrupt();

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();