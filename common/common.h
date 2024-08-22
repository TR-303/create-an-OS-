#pragma once
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;

#define SELECTOR_KERNEL_CODE (0x8 | 0b000)
#define SELECTOR_KERNEL_DATA (0x10 | 0b000)
#define SELECTOR_USER_CODE (0x18 | 0b011)
#define SELECTOR_USER_DATA (0x20 | 0b011)
#define SELECTOR_TSS (0x28 | 0b000)

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

//====================utils======================

void memset(void* ptr, int value, uint32_t num);

void memcpy(void* dest, const void* src, uint32_t n);

void delay(unsigned int ms);

uint32_t min(uint32_t x, uint32_t y);

uint32_t max(uint32_t x, uint32_t y);

//=====================string====================

int32_t strcmp(const char* str1, const char* str2);

char* strcpy(char* dest, const char* src);

int32_t strlen(const char* str);

char* strcat(char* dest, const char* src);

//======================io=======================

void outb(uint16_t port, uint8_t val);

uint8_t inb(uint16_t port);

void outw(uint16_t port, uint16_t val);

uint16_t inw(uint16_t port);

void outsw(uint16_t port, const void* addr, uint32_t count);

void insw(uint16_t port, void* addr, uint32_t count);

//====================bitmap=====================

uint8_t get_bit(uint8_t* bitmap, uint32_t idx);

void set_bit(uint8_t* bitmap, uint32_t idx, uint8_t bit);