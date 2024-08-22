#pragma once
#include "common.h"
#include "int.h"
#include "proc.h"
#define PAGE_TABLE_START (PROC_KSTACK_TABLE_START + 0x1000 * PROC_MAX_CNT)
#define PAGE_BUFFER (uint8_t*)0xc04fe000

typedef struct {
	uint32_t* pde;
	uint32_t* pte;
}pge_t;

void init_paging();

uint32_t page_vir_to_phy(uint32_t addr);

uint32_t page_phy_to_vir(uint32_t addr);

uint32_t sys_alloc_page_frame();

void sys_free_page_frame(uint32_t addr);

void page_fault_handler(isr_param_t* param);

void sys_copy_pages(pcb_t* src, pcb_t* dest);

void sys_cleanup_memory(uint32_t pid);

void sys_create_proc_page_table(uint32_t pid);