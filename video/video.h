#include "common.h"
#include "io.h"

#define VEDIO_START_ADDR 0xc00b8000
#define BIAS_PER_LANE 160

#define COLOR_BLACK 0x0
#define COLOR_BLUE 0x1
#define COLOR_GREEN 0x2
#define COLOR_CYAN 0x3
#define COLOR_RED 0x4
#define COLOR_MAGENTA 0x5
#define COLOR_BROWN 0x6
#define COLOR_LIGHT_GRAY 0x7
#define COLOR_DARK_GRAY 0x8
#define COLOR_LIGHT_BLUE 0x9
#define COLOR_LIGHT_GREEN 0xA
#define COLOR_LIGHT_CYAN 0xB
#define COLOR_LIGHT_RED 0xC
#define COLOR_LIGHT_MAGENTA 0xD
#define COLOR_YELLOW 0xE
#define COLOR_WHITE 0xF

#define COLOR(fg, bg) (((bg) << 4) | (fg))

#define COLOR_DEFAULT COLOR(COLOR_WHITE, COLOR_BLACK)

void set_cursor_pos(uint16_t position);

void set_cursor_coord(uint8_t x, uint8_t y);

uint16_t get_cursor_pos();

void roll_screen();

void putchar_with_color(uint8_t c, uint8_t color);

void putchar(uint8_t c);

void puts_with_color(char *s, uint8_t color);

void puts(char *s);

void clear_screen();
