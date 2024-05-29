#pragma once
#include "common.h"
#include "int.h"

typedef struct interrupt_stack_frame proc_stk_t;

struct pcb
{
    proc_stk_t regs;
    uint32_t pid;
    uint32_t _[256];
};
typedef struct pcb pcb_t;

struct TSS
{
    uint32_t link;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed));
typedef struct TSS tss_t;

extern void resume();

void create_and_start_thread(int pid, char *name, uint32_t addr, uint32_t stack, uint8_t is_user);

void thread_test(uint32_t addr1, uint32_t addr2);
