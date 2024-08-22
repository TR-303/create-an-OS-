#include "timer.h"
#include "schedule.h"
#include "proc.h"
#include "kb.h"

static uint32_t ticks;

void init_timer_interrupt() {
	register_isr_handler(0x20, &timer_handler);
	ticks = 0;
}

void enable_timer_interrupt() {
	uint8_t byte = inb(0x21);
	byte &= 0b11111110;
	outb(0x21, byte);
}

void timer_handler(isr_param_t* param) {
	outb(0x20, 0x20);
	++ticks;
	if(!(ticks & 0b1111))keyboard_handler(param);
	if(!(ticks & 0b11) || proc_table[current_proc]->stat != READY)schedule();
}

int32_t sys_get_ticks(isr_param_t* param) {
	return ticks;
}
