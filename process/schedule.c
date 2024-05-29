#include "schedule.h"
#include "global.h"

void schedule()
{
    if (++proc_running >= &proc_table[2])
        proc_running = &proc_table[0];
    set_tss((uint32_t)proc_running + sizeof(proc_stk_t));
    return;
}