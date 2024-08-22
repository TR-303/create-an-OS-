#pragma once
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;

void memset(void* ptr, int value, uint32_t num);

void memcpy(void* dest, const void* src, uint32_t n);

void delay(unsigned int ms);

uint32_t min(uint32_t x, uint32_t y);

uint32_t max(uint32_t x, uint32_t y);

int32_t strcmp(const char* str1, const char* str2);

char* strcpy(char* dest, const char* src);

int32_t strlen(const char* str);

char* strcat(char* dest, const char* src);