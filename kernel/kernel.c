#include "global.h"
#include "video.h"
#include "kb.h"
#include "timerint.h"
#include "schedule.h"
#include "alloc.h"

void test();
void test2();

void main()
{
    clear_screen();
    set_cursor_coord(0, 0);

    reload_gdt();
    init_interrupt();
    init_timer_interrupt();
    enable_timer_interrupt();
    init_keyboard_interrupt();
    enable_keyboard_interrupt();
    init_syscall();
    init_kernel_alloc();
    init_scheduler();

    start_kernel_thread((uint32_t)&test);
    resume();

    while (1)
    {
    }
}

void test()
{
    while (1)
    {
        char c = read_buffer();
        if (c != EOF)
            putchar(c);
    }
}

void test2()
{
    while (1)
    {
        delay(2);
        putchar('y');
    }
}