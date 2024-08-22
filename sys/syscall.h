#pragma once
#include "common.h"
#include "int.h"

typedef int32_t(*syscall_func_t)(isr_param_t* param);

void init_syscall();

void register_syscall(int num, syscall_func_t func);

int32_t syscall_handler(isr_param_t param);

extern uint32_t syscall();

//===============================================
