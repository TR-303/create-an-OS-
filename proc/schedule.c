#include "schedule.h"
#include "proc.h"
#include "paging.h"
#include "gdt.h"

void schedule() {
    for(uint32_t i = current_proc + 1;;++i) {
        if(i >= PROC_MAX_CNT)i = 0;
        if(proc_table[i]->stat == READY) {
            current_proc = i;
            set_tss(proc_table[current_proc]->kstktop + sizeof(isr_param_t));
            asm volatile("mov %0, %%cr3"::"r"(page_vir_to_phy(proc_table[current_proc]->pde)));
            return;
        } else if(proc_table[i]->stat == DEAD) {
            sys_free_page_frame(proc_table[i]->pde);
            proc_table[i]->stat = EMPTY;
        }
    }
}