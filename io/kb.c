#include "kb.h"
#include "video.h"
#include "tty.h"

uint8_t buffer[32];
uint8_t* head, * tail;
uint32_t cnt = 0;

void init_keyboard_interrupt() {
	cnt = 0;
	memset(buffer, 0, sizeof(buffer));
	tail = head = &buffer[0];
	register_isr_handler(0x21, &keyboard_handler);
}

void enable_keyboard_interrupt() {
	uint8_t byte = inb(0x21);
	byte &= 0b11111101;
	outb(0x21, byte);
}

void keyboard_handler(isr_param_t* param) {
	uint8_t byte;
	while(inb(0x64) & 1) {
		byte = inb(0x60);
		if(cnt >= 32)continue;
		++cnt;
		(*head) = byte;
		if(++head >= &buffer[32])head = &buffer[0];
	}
	outb(0x20, 0x20);
}