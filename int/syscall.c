#include "syscall.h"
#include "global.h"
#include "video.h"

void init_syscall()
{
    set_idt_entry(0x80, (uint32_t)&syscall, (1 << 3) | 0b000, 0b11101110);
    syscalls[0] = &syscall_getticks;
}

uint32_t syscall_handler(isr_param_t param)
{
    int num = param.eax;
    return syscalls[num](param);
}

uint32_t syscall_getticks(isr_param_t param)
{
    return ticks;
}
