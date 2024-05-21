#include "thread.h"
#include "gdt.h"
#include "video.h"

pcb_t proc_table;

void create_and_start_thread(int pid, char *name, uint32_t addr)
{
    memset(&proc_table, 0, sizeof(proc_table));
    proc_table.regs.ds = SELECTOR_USER_DATA;
    proc_table.regs.es = SELECTOR_USER_DATA;
    proc_table.regs.fs = SELECTOR_USER_DATA;
    proc_table.regs.gs = SELECTOR_USER_DATA;

    proc_table.regs.eip = addr;
    proc_table.regs.cs = SELECTOR_USER_CODE;
    proc_table.regs.eflags = 0x3200;
    proc_table.regs.esp = 0x400000;
    proc_table.regs.ss = SELECTOR_USER_DATA;

    proc_table.pid = pid;
    set_tss((uint32_t)&proc_table + 4);

    resume();
}