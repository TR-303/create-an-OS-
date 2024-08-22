#include "interface.h"

int32_t open(const char* name, uint32_t flag) {
	int32_t ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(1), "b"(name), "c"(flag));
	return ret;
}

int32_t close(uint32_t fd) {
	int32_t ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(2), "b"(fd));
	return ret;
}

int32_t write(uint32_t fd, const char* data, uint32_t size) {
	int32_t ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(3), "b"(fd), "c"(data), "d"(size));
	return ret;
}

int32_t read(uint32_t fd, char* dest, uint32_t size) {
	int32_t ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(4), "b"(fd), "c"(dest), "d"(size));
	return ret;
}

int32_t unlink(const char* name) {
	int32_t ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(5), "b"(name));
	return ret;
}

int32_t std_read(char* dest, uint32_t max_size) {
	int32_t ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(6), "b"(dest), "c"(max_size));
	return ret;
}

int32_t std_write(const char* data) {
	int32_t ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(7), "b"(data));
	return ret;
}

int32_t fork() {
	int32_t ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(8));
	return ret;
}

int32_t exit() {
	int32_t ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(9));
	return ret;
}

int32_t wait() {
	int32_t ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(10));
	return ret;
}

int32_t exec(const char* file, uint32_t argc, char* argv[]) {
	int ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(11), "b"(file), "c"(argc), "d"(argv));
	return ret;
}

int32_t getsize(const char* file) {
	int ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(12), "b"(file));
	return ret;
}

int32_t listdir(dirent_t* data) {
	int ret;
	asm volatile("int $0x80":"=a"(ret) : "a"(13), "b"(data));
	return ret;
}
