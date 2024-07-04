#pragma once
#include "common.h"
#include "int.h"

struct PDE
{
    uint32_t pdes[1024];
};
typedef struct PDE pde_t;

struct PTE
{
    uint32_t ptes[1024];
};
typedef struct PTE pte_t;

void init_paging();

void page_fault_handler(isr_param_t param);