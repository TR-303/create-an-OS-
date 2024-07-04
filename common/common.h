#pragma once
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;

#define SELECTOR_KERNEL_CODE (0x8 | 0b000)
#define SELECTOR_KERNEL_DATA (0x10 | 0b000)
#define SELECTOR_VIDEO (0x18 | 0b011)
#define SELECTOR_USER_CODE (0x20 | 0b011)
#define SELECTOR_USER_DATA (0x28 | 0b011)
#define SELECTOR_TSS (0x30 | 0b000)

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

#define EOF -1

void memset(void *ptr, int value, uint32_t num);

void delay(unsigned int ms);

void outb(uint16_t port, uint8_t value);

uint8_t inb(uint16_t port);
