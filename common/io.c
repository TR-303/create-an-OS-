#include "io.h"

void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %1, %0; nop" ::"Nd"(port), "a"(value));
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0; nop" : "=a"(ret) : "Nd"(port));
    return ret;
}