#include "int.h"
#include "video.h"
#include "gdt.h"
#include "thread.h"
#include "common.h"

void testA();
void testB();

void main()
{
    reload_gdt();
    init_interrupt();
    clear_screen();
    set_cursor_coord(0, 0);
    thread_test((uint32_t)&testA, (uint32_t)&testB);

    while (1)
    {
    }
}

void testA()
{
    while (1)
    {
        delay(1);
        putchar('A');
    }
}

void testB()
{
    while (1)
    {
        delay(1);
        putchar('B');
    }
}