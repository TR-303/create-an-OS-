#pragma once
#include "common.h"
#include "int.h"

void init_timer_interrupt();

void enable_timer_interrupt();

void timer_handler(isr_param_t param);