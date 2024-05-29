#include "common.h"

void memset(void *ptr, int value, uint32_t num)
{
    unsigned char *p = (unsigned char *)ptr;
    for (uint32_t i = 0; i < num; i++)
    {
        *p++ = (unsigned char)value;
    }
}

void delay(unsigned int ms)
{
    for (unsigned int i = 0; i < ms; ++i)
    {
        for (int j = 0; j < 1000000; ++j)
            ;
    }
}

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