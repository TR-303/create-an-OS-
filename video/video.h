#pragma once
#include "common.h"

#define VEDIO_START_ADDR 0xc00b8000
#define BIAS_PER_LANE 160

void set_cursor_pos(uint16_t position);

void set_cursor_coord(uint8_t x, uint8_t y);

uint16_t get_cursor_pos();

void roll_screen();

void putchar_with_color(uint8_t c, uint8_t color);

void putchar(uint8_t c);

void puts_with_color(char *s, uint8_t color);

void puts(char *s);

void clear_screen();
