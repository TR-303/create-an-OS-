#include "paging.h"

void init_paging()
{
    register_isr_handler(14, &page_fault_handler);
}

void page_fault_handler(isr_param_t param)
{
}
