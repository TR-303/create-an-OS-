#include "schedule.h"
#include "global.h"
#include "alloc.h"

void init_scheduler()
{
    proc_idx = 0;
    for (int i = 0; i < 10; ++i)
    {
        proc_table[i] = (tcb_t *)(kmalloc() + 0x1000 - sizeof(tcb_t));
        proc_table[i]->stat = EMPTY;
    }
    proc_running = proc_table[0];
}

void schedule()
{
    while (1)
    {
        if (++proc_idx == 10)
            proc_idx = 0;
        proc_running = proc_table[proc_idx];
        if (proc_running->stat == RUNNING)
            break;
    }
}