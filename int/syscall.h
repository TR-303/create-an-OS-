#pragma once
#include "common.h"
#include "int.h"

typedef uint32_t (*syscall_ptr_t)(isr_param_t);

void init_syscall();

uint32_t syscall_handler(isr_param_t param);

uint32_t syscall_getticks(isr_param_t param);

extern uint32_t syscall();

extern uint32_t get_ticks();