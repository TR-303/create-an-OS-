#include "utils.h"

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
