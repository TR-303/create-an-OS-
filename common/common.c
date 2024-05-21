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