#pragma once
#include "common.h"
#include "int.h"

#define FS_BUFFER (uint8_t*)0xc04ff000

#define ATA_PRIMARY_IO_BASE 0x1F0
#define ATA_PRIMARY_CONTROL 0x3F6
#define ATA_SECONDARY_IO_BASE 0x170
#define ATA_SECONDARY_CONTROL 0x376

#define ATA_CMD_IDENTIFY 0xEC

typedef struct SuperBlock {
	uint32_t magic;
	char name[8];
	uint32_t inode_count;
	uint32_t sector_count;
	uint32_t sector_map_start;
	uint32_t inode_map_start;
	uint32_t inode_table_start;
	uint32_t data_start;
}superblock_t;

#define ITYPE_DIR 1
#define ITYPE_FILE 2

typedef struct Inode {
	uint32_t type;
	uint32_t size;
	uint32_t block_count;
	uint32_t direct_pointers[29];
}inode_t;

typedef struct DirectoryEntry {
	uint32_t inode_idx;
	char base[8];
	char ext[4];
}dir_entry_t;

void init_fs();

void read_sector(uint32_t lba, uint8_t* buffer);

void write_sector(uint32_t lba, const uint8_t* buffer);

void sys_format();

typedef struct {
	inode_t* inode;
	uint32_t rwpointer;
	uint32_t rwcount;
}file_desc_t;

#define OPEN_CREATE 1
#define OPEN_EXIST 2
#define OPEN_APPEND 4

int32_t sys_open(isr_param_t* param);

int32_t sys_do_open(const char* name, uint32_t flag);

int32_t sys_close(isr_param_t* param);

int32_t sys_do_close(uint32_t fd);

int32_t sys_write(isr_param_t* param);

int32_t sys_do_write(uint32_t fd, const char* data, uint32_t size);

int32_t sys_read(isr_param_t* param);

int32_t sys_do_read(uint32_t fd, char* dest, uint32_t size);

int32_t sys_unlink(isr_param_t* param);

int32_t sys_do_unlink(const char* name);

int32_t sys_getsize(isr_param_t* param);

int32_t sys_do_getsize(const char* name);