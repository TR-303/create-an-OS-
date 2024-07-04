#include "timerint.h"
#include "global.h"
#include "video.h"
#include "schedule.h"

void init_timer_interrupt()
{
    register_isr_handler(0x20, &timer_handler);
}

void enable_timer_interrupt()
{
    uint8_t byte = inb(0x21);
    byte &= 0b11111110;
    outb(0x21, byte);
}

void timer_handler(isr_param_t param)
{
    // reenable 8259A
    outb(0x20, 0x20);
    ++ticks;
    schedule();
}