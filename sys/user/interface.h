#pragma once
#include "common.h"

int32_t open(const char* name, uint32_t flag);

int32_t close(uint32_t fd);

int32_t write(uint32_t fd, const char* data, uint32_t size);

int32_t read(uint32_t fd, char* dest, uint32_t size);

int32_t unlink(const char* name);

int32_t std_read(char* dest, uint32_t max_size);

int32_t std_write(const char* data);

int32_t fork();

int32_t exit();

int32_t wait();

int32_t exec(const char* file, uint32_t argc, char* argv[]);

int32_t getsize(const char* file);

typedef struct dirent {
	char fullname[16];
	uint32_t size;
	uint32_t space;
}dirent_t;

int32_t listdir(dirent_t* data);