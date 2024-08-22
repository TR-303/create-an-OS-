#pragma once
#include "common.h"
#include "int.h"

extern uint8_t buffer[32];
extern uint8_t* head, * tail;
extern uint32_t cnt;


void init_keyboard_interrupt();

void enable_keyboard_interrupt();

void keyboard_handler(isr_param_t* param);