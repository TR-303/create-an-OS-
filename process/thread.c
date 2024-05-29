#include "thread.h"
#include "global.h"

void create_and_start_thread(int pid, char *name, uint32_t addr, uint32_t stack, uint8_t is_user)
{
    memset(&proc_table, 0, sizeof(proc_table));
    proc_table[pid].regs.ds = SELECTOR_USER_DATA;
    proc_table[pid].regs.es = SELECTOR_USER_DATA;
    proc_table[pid].regs.fs = SELECTOR_USER_DATA;
    proc_table[pid].regs.gs = SELECTOR_USER_DATA;

    proc_table[pid].regs.eip = addr;
    proc_table[pid].regs.cs = SELECTOR_USER_CODE;
    proc_table[pid].regs.eflags = is_user ? 0x3200 : 0x200;
    proc_table[pid].regs.esp = stack;
    proc_table[pid].regs.ss = SELECTOR_USER_DATA;

    proc_table[pid].pid = pid;
    set_tss((uint32_t)&proc_table + 4);

    resume();
}

void thread_test(uint32_t addr1, uint32_t addr2)
{
    memset(&proc_table, 0, sizeof(proc_table));
    proc_table[0].regs.ds = SELECTOR_USER_DATA;
    proc_table[0].regs.es = SELECTOR_USER_DATA;
    proc_table[0].regs.fs = SELECTOR_USER_DATA;
    proc_table[0].regs.gs = SELECTOR_USER_DATA;

    proc_table[0].regs.eip = addr1;
    proc_table[0].regs.cs = SELECTOR_USER_CODE;
    proc_table[0].regs.eflags = 0x3200;
    proc_table[0].regs.esp = 0x380000;
    proc_table[0].regs.ss = SELECTOR_USER_DATA;

    proc_table[0].pid = 0;

    proc_table[1].regs.ds = SELECTOR_USER_DATA;
    proc_table[1].regs.es = SELECTOR_USER_DATA;
    proc_table[1].regs.fs = SELECTOR_USER_DATA;
    proc_table[1].regs.gs = SELECTOR_USER_DATA;

    proc_table[1].regs.eip = addr2;
    proc_table[1].regs.cs = SELECTOR_USER_CODE;
    proc_table[1].regs.eflags = 0x3200;
    proc_table[1].regs.esp = 0x360000;
    proc_table[1].regs.ss = SELECTOR_USER_DATA;

    proc_table[1].pid = 1;

    set_tss((uint32_t)proc_running + sizeof(proc_stk_t));

    enable_interrupt();

    resume();
}