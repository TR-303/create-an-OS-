#include "common.h"

//====================utils======================

void memset(void* ptr, int value, uint32_t num) {
	uint8_t* p = (uint8_t*)ptr;
	for(uint32_t i = 0; i < num; i++)*p++ = (uint8_t)value;
}

void memcpy(void* dest, const void* src, uint32_t n) {
	uint8_t* pd = (uint8_t*)dest, * ps = (uint8_t*)src;
	for(uint32_t i = 0; i < n; i++)pd[i] = ps[i];
}

void delay(unsigned int ms) {
	for(unsigned int i = 0; i < ms; ++i) {
		for(int j = 0; j < 1000; ++j);
	}
}

inline uint32_t min(uint32_t x, uint32_t y) {
	if(x < y)return x;
	else return y;
}

inline uint32_t max(uint32_t x, uint32_t y) {
	if(x > y)return x;
	else return y;
}

//=====================string====================

int32_t strcmp(const char* str1, const char* str2) {
	while(*str1 && (*str1 == *str2)) {
		str1++;
		str2++;
	}
	return *(unsigned char*)str1 - *(unsigned char*)str2;
}

char* strcpy(char* dest, const char* src) {
	char* ret = dest;
	while((*dest++ = *src++));
	return ret;
}

int32_t strlen(const char* str) {
	const char* s = str;
	while(*s) {
		s++;
	}
	return s - str;
}

char* strcat(char* dest, const char* src) {
	char* ret = dest;
	while(*dest) {
		dest++;
	}
	while((*dest++ = *src++));
	return ret;
}

//======================io=======================

inline void outb(uint16_t port, uint8_t val) {
	asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

inline void outw(uint16_t port, uint16_t val) {
	asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

inline uint16_t inw(uint16_t port) {
	uint16_t ret;
	asm volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

inline void outsw(uint16_t port, const void* addr, uint32_t count) {
	asm volatile ("rep outsw" : "+S"(addr), "+c"(count) : "d"(port));
}

inline void insw(uint16_t port, void* addr, uint32_t count) {
	asm volatile ("rep insw" : "+D"(addr), "+c"(count) : "d"(port));
}

//====================bitmap=====================

uint8_t get_bit(uint8_t* bitmap, uint32_t idx) {
	return (bitmap[idx / 8] >> (idx & 0b111)) & 1;
}

void set_bit(uint8_t* bitmap, uint32_t idx, uint8_t bit) {
	bitmap[idx / 8] = (bitmap[idx / 8] & ~(1 << (idx & 0b111))) | (bit << (idx & 0b111));
}