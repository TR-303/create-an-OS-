#include "int.h"
#include "io.h"
#include "video.h"

int16_t reenter = 0;

idt_entry_t idt[256];
idt_ptr_t idt_ptr;

isr_handler_ptr_t isr_handlers[256] = {};

void enable_interrupt()
{
    asm volatile("sti");
}

void diable_interrupt()
{
    asm volatile("cli");
}

void isr_handler(isr_param_t param)
{
    if (isr_handlers[param.num] == 0)
    {
        set_cursor_coord(0, 0);
        char str[26] = "Unregisterd interrupt ";
        str[22] = '0' + param.num / 10;
        str[23] = '0' + param.num % 10;
        str[24] = '!';
        str[25] = '\0';
        puts_with_color(str, COLOR(COLOR_RED, COLOR_GREEN));
        while (1)
            ;
    }
    else
        isr_handlers[param.num](param);
}

void timer_handler(isr_param_t param)
{
    outb(0x20, 0x20);
    putchar('T');
}

void page_fault_handler(isr_param_t param)
{
    puts("page fault!\n");
}

void set_idt_entry(int num, uint32_t addr, uint16_t selector, uint8_t attributes)
{
    idt[num].addr_low = addr & 0xffff;
    idt[num].selector = selector;
    idt[num].reserved = 0;
    idt[num].attributes = attributes;
    idt[num].addr_high = addr >> 16;
}

void register_isr_handler(int num, isr_handler_ptr_t handler)
{
    isr_handlers[num] = handler;
}

void init_interrupt()
{
    // init 8259A
    outb(0x20, 0x11);
    outb(0xa0, 0x11);
    outb(0x21, 0x20);
    outb(0xa1, 0x28);
    outb(0x21, 0x04);
    outb(0xa1, 0x02);
    outb(0x21, 0x01);
    outb(0xa1, 0x01);
    outb(0x21, 0b11111110);
    outb(0xa1, 0b11111111);

    set_idt_entry(0, (uint32_t)isr0, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(1, (uint32_t)isr1, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(2, (uint32_t)isr2, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(3, (uint32_t)isr3, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(4, (uint32_t)isr4, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(5, (uint32_t)isr5, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(6, (uint32_t)isr6, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(7, (uint32_t)isr7, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(8, (uint32_t)isr8, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(9, (uint32_t)isr9, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(10, (uint32_t)isr10, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(11, (uint32_t)isr11, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(12, (uint32_t)isr12, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(13, (uint32_t)isr13, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(14, (uint32_t)isr14, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(15, (uint32_t)isr15, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(16, (uint32_t)isr16, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(17, (uint32_t)isr17, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(18, (uint32_t)isr18, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(19, (uint32_t)isr19, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(20, (uint32_t)isr20, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(21, (uint32_t)isr21, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(22, (uint32_t)isr22, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(23, (uint32_t)isr23, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(24, (uint32_t)isr24, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(25, (uint32_t)isr25, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(26, (uint32_t)isr26, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(27, (uint32_t)isr27, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(28, (uint32_t)isr28, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(29, (uint32_t)isr29, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(30, (uint32_t)isr30, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(31, (uint32_t)isr31, (1 << 3) | 0b000, 0b10001110);

    set_idt_entry(32, (uint32_t)isr32, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(33, (uint32_t)isr33, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(34, (uint32_t)isr34, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(35, (uint32_t)isr35, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(36, (uint32_t)isr36, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(37, (uint32_t)isr37, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(38, (uint32_t)isr38, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(39, (uint32_t)isr39, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(40, (uint32_t)isr40, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(41, (uint32_t)isr41, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(42, (uint32_t)isr42, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(43, (uint32_t)isr43, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(44, (uint32_t)isr44, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(45, (uint32_t)isr45, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(46, (uint32_t)isr46, (1 << 3) | 0b000, 0b10001110);
    set_idt_entry(47, (uint32_t)isr47, (1 << 3) | 0b000, 0b10001110);

    register_isr_handler(32, &timer_handler);
    register_isr_handler(14, &page_fault_handler);

    idt_ptr.base = (uint32_t)idt;
    idt_ptr.limit = (256 << 3) - 1;
    asm volatile("lidt [%0]"
                 :
                 : "m"(idt_ptr));
}