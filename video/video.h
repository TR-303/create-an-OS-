#pragma once
#include "common.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

void sys_set_cursor_pos(uint32_t tty_num, uint16_t position);

void sys_set_cursor_coord(uint32_t tty_num, uint8_t x, uint8_t y);

uint16_t get_cursor_pos(uint32_t tty_num);

void sys_roll_screen(uint32_t tty_num);

void sys_putchar_with_color(uint32_t tty_num, uint8_t c, uint8_t color);

void sys_putchar(uint32_t tty_num, uint8_t c);

void sys_puts_with_color(uint32_t tty_num, const char* s, uint8_t color);

void sys_puts(uint32_t tty_num, const char* s);

void sys_clear_screen(uint32_t tty_num);