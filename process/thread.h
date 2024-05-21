#include "common.h"

extern void resume();

struct stack_frame
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, kesp, ebx, edx, ecx, eax;
    uint32_t ret_addr;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
};
typedef struct stack_frame stack_frame_t;

struct pcb
{
    stack_frame_t regs;
    uint32_t pid;
    char name[32];
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

void create_and_start_thread(int pid, char *name, uint32_t addr);
