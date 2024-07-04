#include "alloc.h"

uint32_t kmbitmap[8];

uint32_t mbitmap[0xf00];

void init_kernel_alloc()
{
    for (int i = 0; i < 4; ++i)
        kmbitmap[i] = 0;
    for (int i = 0; i < 0xf00; ++i)
        mbitmap[i] = 0;
}

void *kmalloc()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 32; ++j)
            if ((kmbitmap[i] & (1 << j)) == 0)
            {
                kmbitmap[i] |= (1 << j);
                return (void *)(0xc0500000 + (i * 32 + j) * 0x1000);
            }
    }
}

void *malloc()
{
    for (int i = 0; i < 0x80 / 32; ++i)
    {
        for (int j = 0; j < 32; ++j)
            if ((kmbitmap[i] & (1 << j)) == 0)
            {
                kmbitmap[i] |= (1 << j);
                return 0;
            }
    }
}