#include "int.h"
#include "video.h"
#include "kb.h"
#include "timer.h"
#include "gdt.h"
#include "alloc.h"
#include "paging.h"
#include "syscall.h"
#include "proc.h"
#include "schedule.h"
#include "fs.h"
#include "tty.h"

void install() {
	uint8_t* elf = EXEC_BUFFER;
	for(uint32_t i = 0;i < 20;++i)read_sector(i + 1, elf + 0x200 * i);
	int fp = sys_do_open("shell.elf", OPEN_CREATE | OPEN_EXIST);
	if(fp == -1) {
		sys_puts(current_proc, "open failed");
		while(1);
	}
	if(sys_do_write(fp, elf, 20 * 0x200) == -1) {
		sys_puts(current_proc, "write failed");
		while(1);
	};
	sys_do_close(fp);
	sys_puts(current_proc, "done");
	while(1);
}

void main() {
	disable_interrupt();

	reload_gdt();

	init_interrupt();
	init_timer_interrupt();
	init_keyboard_interrupt();
	enable_timer_interrupt();
	enable_keyboard_interrupt();

	init_syscall();
	init_proc();
	init_memory();
	init_paging();
	init_fs();
	init_ttys();

	sys_clear_screen(0);
	sys_set_cursor_coord(0, 0, 0);

	// sys_setup_kernel_process(0, &install);
	setup_init();
	sys_setup_kernel_process(1, &tty_driver);

	schedule();
	resume();

	while(1);
}
