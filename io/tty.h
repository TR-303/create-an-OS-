#pragma once
#include "common.h"
#include "int.h"

typedef struct TTY {
	uint16_t* addr;
	uint16_t pos;
	uint8_t in[128];
	//uint8_t out[512];
	uint8_t* head, * tail;
	uint32_t cnt;
}tty_t;

extern tty_t ttys[2];
extern uint32_t current_tty;

void init_ttys();

void sys_switch_tty(uint32_t i);

void tty_driver();

int32_t sys_std_read(isr_param_t* param);

int32_t sys_std_write(isr_param_t* param);