#include "syscall.h"
#include "video.h"
#include "kb.h"
#include "tty.h"
#include "gdt.h"

static syscall_func_t syscall_func[64];

void init_syscall() {
	set_idt_entry(0x80, (uint32_t)&syscall, SELECTOR_KERNEL_CODE, 0b11101110);
	memset(syscall_func, 0, sizeof(syscall_func));
}

void register_syscall(int num, syscall_func_t func) {
	syscall_func[num] = func;
}

int32_t syscall_handler(isr_param_t param) {
	if(syscall_func[param.eax] == 0) {
		sys_set_cursor_coord(current_tty, 0, 0);
		char str[] = "Unregisterd syscall!";
		sys_puts_with_color(current_tty, str, COLOR(COLOR_RED, COLOR_GREEN));
		while(1);
	} else {
		int32_t ret = syscall_func[param.eax](&param);
		set_tss((uint32_t)&param + sizeof(param));
		return ret;
	}
}
