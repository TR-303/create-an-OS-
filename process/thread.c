#include "thread.h"
#include "global.h"
#include "alloc.h"

void start_kernel_thread(uint32_t func)
{
    int idx = 0;
    while (proc_table[idx]->stat != EMPTY)
        ++idx;
    tcb_t *stk = proc_table[idx];
    memset(stk, 0, sizeof(tcb_t));
    stk->regs.cs = SELECTOR_KERNEL_CODE;
    stk->regs.ds = SELECTOR_KERNEL_DATA;
    stk->regs.es = SELECTOR_KERNEL_DATA;
    stk->regs.fs = SELECTOR_KERNEL_DATA;
    stk->regs.gs = SELECTOR_KERNEL_DATA;
    stk->regs.eip = func;
    stk->stktop = (uint32_t)stk;
    stk->regs.eflags = 0x202;
    stk->stat = RUNNING;
}

void start_user_thread(uint32_t func)
{
}