#include "int.h"
#include "kb.h"
#include "video.h"
#include "tty.h"

idt_entry_t idt[256];
idt_ptr_t idt_ptr;
isr_handler_ptr_t isr_handlers[256];

void init_interrupt() {
	// init 8259A
	outb(0x20, 0x11);
	outb(0xa0, 0x11);
	outb(0x21, 0x20);
	outb(0xa1, 0x28);
	outb(0x21, 0x04);
	outb(0xa1, 0x02);
	outb(0x21, 0x01);
	outb(0xa1, 0x01);
	outb(0x21, 0b11111111);
	outb(0xa1, 0b11111111);

	memset(idt, 0, sizeof(idt));
	memset(isr_handlers, 0, sizeof(isr_handlers));

	set_idt_entry(0, (uint32_t)isr0, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(1, (uint32_t)isr1, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(2, (uint32_t)isr2, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(3, (uint32_t)isr3, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(4, (uint32_t)isr4, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(5, (uint32_t)isr5, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(6, (uint32_t)isr6, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(7, (uint32_t)isr7, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(8, (uint32_t)isr8, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(9, (uint32_t)isr9, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(10, (uint32_t)isr10, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(11, (uint32_t)isr11, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(12, (uint32_t)isr12, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(13, (uint32_t)isr13, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(14, (uint32_t)isr14, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(15, (uint32_t)isr15, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(16, (uint32_t)isr16, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(17, (uint32_t)isr17, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(18, (uint32_t)isr18, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(19, (uint32_t)isr19, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(20, (uint32_t)isr20, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(21, (uint32_t)isr21, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(22, (uint32_t)isr22, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(23, (uint32_t)isr23, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(24, (uint32_t)isr24, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(25, (uint32_t)isr25, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(26, (uint32_t)isr26, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(27, (uint32_t)isr27, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(28, (uint32_t)isr28, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(29, (uint32_t)isr29, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(30, (uint32_t)isr30, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(31, (uint32_t)isr31, SELECTOR_KERNEL_CODE, 0b11101110);

	set_idt_entry(32, (uint32_t)isr32, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(33, (uint32_t)isr33, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(34, (uint32_t)isr34, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(35, (uint32_t)isr35, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(36, (uint32_t)isr36, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(37, (uint32_t)isr37, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(38, (uint32_t)isr38, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(39, (uint32_t)isr39, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(40, (uint32_t)isr40, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(41, (uint32_t)isr41, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(42, (uint32_t)isr42, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(43, (uint32_t)isr43, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(44, (uint32_t)isr44, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(45, (uint32_t)isr45, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(46, (uint32_t)isr46, SELECTOR_KERNEL_CODE, 0b11101110);
	set_idt_entry(47, (uint32_t)isr47, SELECTOR_KERNEL_CODE, 0b11101110);

	idt_ptr.base = (uint32_t)idt;
	idt_ptr.limit = (256 << 3) - 1;
	asm volatile("lidt [%0]"
		:
	: "m"(idt_ptr));
}

void enable_interrupt() {
	asm volatile("sti");
}

void disable_interrupt() {
	asm volatile("cli");
}

void set_idt_entry(uint32_t num, uint32_t addr, uint16_t selector, uint8_t attributes) {
	idt[num].addr_low = addr & 0xffff;
	idt[num].selector = selector;
	idt[num].reserved = 0;
	idt[num].attributes = attributes;
	idt[num].addr_high = addr >> 16;
}

void register_isr_handler(uint32_t num, isr_handler_ptr_t handler) {
	isr_handlers[num] = handler;
}

void isr_handler(isr_param_t param) {
	if(isr_handlers[param.num] == 0) {
		sys_set_cursor_coord(current_tty, 0, 0);
		char str[39] = "Unregisterd interrupt ";
		str[22] = '0' + param.num / 10;
		str[23] = '0' + param.num % 10;
		str[24] = ' ';
		str[25] = 'a';
		str[26] = 't';
		str[27] = ' ';
		str[28] = '0';
		str[29] = 'x';
		uint32_t eip = param.eip;
		for(uint32_t i = 0;i < 8;++i) {
			uint8_t c = (eip >> (i * 4)) & 0xf;
			if(c < 10)str[37 - i] = '0' + c;
			else str[37 - i] = 'a' - 10 + c;
		}
		str[38] = '\0';
		sys_puts_with_color(current_tty, str, COLOR(COLOR_RED, COLOR_GREEN));
		while(1);
	} else
		isr_handlers[param.num](&param);
}