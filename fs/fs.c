#include "fs.h"
#include "proc.h"
#include "syscall.h"
#include "user/interface.h"

static superblock_t meta;

static file_desc_t file_desc_table[64];

static inode_t inode_table[64];

static uint8_t inode_map[8];

void init_fs() {
	register_syscall(1, &sys_open);
	register_syscall(2, &sys_close);
	register_syscall(3, &sys_write);
	register_syscall(4, &sys_read);
	register_syscall(5, &sys_unlink);
	register_syscall(12, &sys_getsize);
	register_syscall(13, &sys_listdir);

	outb(0x1F6, 0xA0 | (0 << 4));

	memset(file_desc_table, 0, sizeof(file_desc_table));
	memset(inode_table, 0, sizeof(inode_table));

	read_sector(0, FS_BUFFER);
	memcpy(&meta, FS_BUFFER, sizeof(meta));
	if(meta.magic != 0xBABA)sys_format();

	read_sector(meta.inode_map_start, FS_BUFFER);
	memcpy(inode_map, FS_BUFFER, sizeof(inode_map));

	for(uint32_t block = meta.inode_table_start, i = 0;block < meta.data_start;++block, i += 4)read_sector(block, (uint8_t*)&inode_table[i]);
}

void read_sector(uint32_t lba, uint8_t* buffer) {
	outb(0x1F6, 0xE0 | ((lba >> 24) & 0xF));
	outb(0x1F2, 1);
	outb(0x1F3, (uint8_t)lba);
	outb(0x1F4, (uint8_t)(lba >> 8));
	outb(0x1F5, (uint8_t)(lba >> 16));
	outb(0x1F7, 0x20);

	while(inb(0x1F7) & 0x80);

	insw(0x1F0, buffer, 256);
}

void write_sector(uint32_t lba, const uint8_t* buffer) {
	outb(0x1F6, 0xE0 | ((lba >> 24) & 0xF));
	outb(0x1F2, 1);
	outb(0x1F3, (uint8_t)lba);
	outb(0x1F4, (uint8_t)(lba >> 8));
	outb(0x1F5, (uint8_t)(lba >> 16));
	outb(0x1F7, 0x30);

	while(inb(0x1F7) & 0x80);

	outsw(0x1F0, buffer, 256);
}

uint32_t get_sectors() {
	uint16_t buffer[256];

	outb(0x1f7, 0xec);
	while(inb(0x1F7) & 0x80);

	if(inb(0x1f7) == 0) while(1);

	insw(0x1f0, buffer, 256);

	return buffer[60] | (buffer[61] << 16);
}

void sys_format() {
	memset(FS_BUFFER, 0, 512);
	meta.magic = 0xBABA;
	meta.name[0] = 'B';
	meta.name[1] = 'a';
	meta.name[2] = 'n';
	meta.name[3] = 'a';
	meta.name[4] = 'n';
	meta.name[5] = 'a';
	meta.name[6] = '\0';
	meta.name[7] = '\0';
	meta.inode_count = 64;
	meta.sector_count = get_sectors();
	meta.sector_map_start = 32;
	meta.inode_map_start = meta.sector_map_start + (meta.sector_count - 1) / 512 + 1;
	meta.inode_table_start = meta.inode_map_start + (meta.inode_count - 1) / 512 + 1;
	meta.data_start = meta.inode_table_start + (meta.inode_count - 1) / 4 + 1;

	memcpy(FS_BUFFER, &meta, sizeof(meta));

	write_sector(0, FS_BUFFER);

	memset(FS_BUFFER, 0, 512);
	for(uint32_t i = meta.sector_map_start;i < meta.inode_table_start;++i)write_sector(i, FS_BUFFER);

	inode_t* root = (inode_t*)FS_BUFFER;
	root->type = ITYPE_DIR;
	root->block_count = 2;
	root->size = 1024;
	root->direct_pointers[0] = 0;
	root->direct_pointers[1] = 1;

	write_sector(meta.inode_table_start, FS_BUFFER);

	memset(FS_BUFFER, 0, 512);
	uint8_t* p = FS_BUFFER;
	(*p) = 0x1;
	write_sector(meta.inode_map_start, p);

	(*p) = 0x03;
	write_sector(meta.sector_map_start, p);

	memset(FS_BUFFER, 0xff, 512);
	for(uint32_t i = 0;i < 2;++i)write_sector(meta.data_start + i, FS_BUFFER);
}

void split_name(const char* name, char* base, char* extension) {
	int len = 0;
	int dot = -1;

	while(name[len] != '\0') {
		if(name[len] == '.')dot = len;
		len++;
	}

	if(dot == -1) {
		for(int i = 0; i < len && i < 7; i++)base[i] = name[i];
		base[len] = base[7] = '\0';
		extension[0] = '\0';
	} else {
		for(int i = 0; i < dot && i < 7; i++)base[i] = name[i];
		base[dot] = base[7] = '\0';

		int extidx = 0;
		for(int i = dot + 1; i < len && extidx < 3; i++) {
			extension[extidx] = name[i];
			extidx++;
		}
		extension[extidx] = extension[3] = '\0';
	}
}

void sync_inode() {
	memset(FS_BUFFER, 0, 512);
	memcpy(FS_BUFFER, inode_map, sizeof(inode_map));
	write_sector(meta.inode_map_start, FS_BUFFER);
	for(uint32_t block = meta.inode_table_start, i = 0;block < meta.data_start;++block, i += 4)write_sector(block, (uint8_t*)&inode_table[i]);
}

int32_t alloc_sector() {
	for(uint32_t block = meta.sector_map_start;block < meta.inode_map_start;++block) {
		read_sector(block, FS_BUFFER);
		uint8_t* sector_map = FS_BUFFER;
		for(uint32_t i = 0; i < 512;++i)if(get_bit(sector_map, i) == 0) {
			set_bit(sector_map, i, 1);
			write_sector(block, FS_BUFFER);
			return (block - meta.sector_map_start) * 512 + i;
		}
	}
	return -1;
}

dir_entry_t find_file(const char* name) {
	for(uint32_t block = 0;block < inode_table[0].block_count;++block) {
		read_sector(meta.data_start + block, FS_BUFFER);
		dir_entry_t* dirs = (dir_entry_t*)FS_BUFFER;
		for(uint32_t i = 0;i < 32;++i) {
			if(dirs[i].inode_idx == 0xffffffff)continue;
			char filename[16];
			strcpy(filename, dirs[i].base);
			if(dirs[i].ext[0] != '\0') {
				strcat(filename, ".");
				strcat(filename, dirs[i].ext);
			}
			if(strcmp(filename, name) == 0)return dirs[i];
		}
	}
	return (dir_entry_t) { 0xffffffff };
}

int32_t create_file(const char* name) {
	for(uint32_t block = 0;block < inode_table[0].block_count;++block) {
		read_sector(meta.data_start + block, FS_BUFFER);
		dir_entry_t* dirs = (dir_entry_t*)FS_BUFFER;
		for(uint32_t i = 0;i < 32;++i)if(dirs[i].inode_idx == 0xffffffff) {
			split_name(name, dirs[i].base, dirs[i].ext);
			for(uint32_t j = 0;j < 64;++j)if(get_bit(inode_map, j) == 0) {
				set_bit(inode_map, j, 1);
				memset(&inode_table[j], 0, sizeof(inode_t));
				inode_table[j].type = ITYPE_FILE;
				dirs[i].inode_idx = j;
				write_sector(meta.data_start + block, FS_BUFFER);
				sync_inode();
				return j;
			}
			break;
		}
	}
	return -1;
}

int32_t sys_open(isr_param_t* param) {
	const char* name = (const char*)param->ebx;
	uint32_t flag = (uint32_t)param->ecx;
	return sys_do_open(name, flag);
}

int32_t sys_do_open(const char* name, uint32_t flag) {
	int32_t fp = -1;
	for(uint32_t fpi = 0;fpi < 4;++fpi)if(proc_table[current_proc]->fds[fpi] == 0) {
		fp = fpi;
		break;
	}
	if(fp == -1)while(1);

	uint32_t fdi;
	for(fdi = 0;fdi < 64;++fdi)if(file_desc_table[fdi].inode == 0)break;
	if(fdi >= 64)while(1);

	int32_t existing = find_file(name).inode_idx;
	if(!(flag & OPEN_CREATE) && existing < 0)return -1;
	if(!(flag & OPEN_EXIST) && existing >= 0)return -1;
	if(existing < 0) {
		existing = create_file(name);
		if(existing < 0)return -1;
	}
	proc_table[current_proc]->fds[fp] = &file_desc_table[fdi];
	file_desc_table[fdi].inode = &inode_table[existing];
	file_desc_table[fdi].rwpointer = 0;
	++file_desc_table[fdi].rwcount;
	if(flag & OPEN_APPEND)file_desc_table[fdi].rwpointer = inode_table[existing].size;
	return fp;
}

int32_t sys_close(isr_param_t* param) {
	uint32_t fd = (uint32_t)param->ebx;
	return sys_do_close(fd);
}

int32_t sys_do_close(uint32_t fd) {
	if(fd >= 4)while(1);

	file_desc_t* fdp = proc_table[current_proc]->fds[fd];
	proc_table[current_proc]->fds[fd] = 0;
	if(fdp == 0)return -1;
	inode_t* inp = fdp->inode;
	if(--fdp->rwcount == 0)fdp->inode = 0;
	if(inp == 0)return -1;
	return 0;
}

int32_t sys_write(isr_param_t* param) {
	uint32_t fd = (uint32_t)param->ebx;
	const char* data = (const char*)param->ecx;
	uint32_t size = (uint32_t)param->edx;
	return sys_do_write(fd, data, size);
}

int32_t sys_do_write(uint32_t fd, const char* data, uint32_t size) {
	if(fd >= 4)while(1);

	file_desc_t* fdp = proc_table[current_proc]->fds[fd];
	if(fdp == 0)return -1;

	inode_t* inode = fdp->inode;
	if(inode == 0)return -1;

	uint32_t block_start = fdp->rwpointer / 512, block_end = (fdp->rwpointer + size) / 512;
	uint32_t pre = fdp->rwpointer % 512;
	while(block_end >= inode->block_count) {
		if(inode->block_count >= 29)return -1;
		inode->direct_pointers[inode->block_count++] = alloc_sector();
	}

	uint32_t* blocks = inode->direct_pointers;

	read_sector(meta.data_start + blocks[block_start], FS_BUFFER);
	memcpy(FS_BUFFER + pre, data, min(size, 512 - pre));
	write_sector(meta.data_start + blocks[block_start], FS_BUFFER);

	if(block_end - block_start > 0) {
		const uint8_t* p = data + min(size, 512 - pre);
		for(uint32_t i = block_start + 1;i < block_end;++i, p += 512)write_sector(meta.data_start + blocks[i], p);

		read_sector(meta.data_start + blocks[block_end], FS_BUFFER);
		memcpy(FS_BUFFER, data, (size + pre) % 512);
		write_sector(meta.data_start + blocks[block_end], FS_BUFFER);
	}

	inode->size = max(inode->size, fdp->rwpointer + size);

	sync_inode();

	return size;
}

int32_t sys_read(isr_param_t* param) {
	uint32_t fd = (uint32_t)param->ebx;
	char* dest = (char*)param->ecx;
	uint32_t size = (uint32_t)param->edx;
	return sys_do_read(fd, dest, size);
}

int32_t sys_do_read(uint32_t fd, char* dest, uint32_t size) {
	if(fd >= 4)while(1);

	file_desc_t* fdp = proc_table[current_proc]->fds[fd];
	if(fdp == 0)return -1;

	inode_t* inode = fdp->inode;
	if(inode == 0)return -1;

	uint32_t block_start = fdp->rwpointer / 512;
	size = min(size, inode->size - fdp->rwpointer);
	uint32_t block_end = (fdp->rwpointer + size) / 512;
	uint32_t pre = fdp->rwpointer % 512;

	uint32_t* blocks = inode->direct_pointers;

	read_sector(meta.data_start + blocks[block_start], FS_BUFFER);
	memcpy(dest, FS_BUFFER + pre, min(size, 512 - pre));

	if(block_end - block_start > 0) {
		uint8_t* p = dest + min(size, 512 - pre);
		for(uint32_t i = block_start + 1;i < block_end;++i, p += 512)read_sector(meta.data_start + blocks[i], p);

		read_sector(meta.data_start + blocks[block_end], FS_BUFFER);
		memcpy(p, FS_BUFFER, (size + pre) % 512);
	}

	return size;
}

int32_t sys_unlink(isr_param_t* param) {
	const char* name = (const char*)param->ebx;
	return sys_do_unlink(name);
}

int32_t sys_do_unlink(const char* name) {
	for(uint32_t block = 0;block < inode_table[0].block_count;++block) {
		read_sector(meta.data_start + block, FS_BUFFER);
		dir_entry_t* dirs = (dir_entry_t*)FS_BUFFER;
		for(uint32_t i = 0;i < 32;++i) {
			if(dirs[i].inode_idx == 0xffffffff)continue;
			char filename[16];
			strcpy(filename, dirs[i].base);
			if(dirs[i].ext[0] != '\0') {
				strcat(filename, ".");
				strcat(filename, dirs[i].ext);
			}
			if(strcmp(filename, name) == 0) {
				dirs[i].inode_idx = 0xffffffff;
				write_sector(meta.data_start + block, FS_BUFFER);
				inode_t* inode = &inode_table[dirs[i].inode_idx];
				set_bit(inode_map, dirs[i].inode_idx, 0);
				for(uint32_t j = 0;j < inode->block_count;++j) {
					uint32_t sector = inode->direct_pointers[j];
					read_sector(meta.sector_map_start + sector / 512, FS_BUFFER);
					set_bit(FS_BUFFER, sector % 512, 0);
					write_sector(meta.sector_map_start + sector / 512, FS_BUFFER);
				}
				return 0;
			};
		}
	}

	return -1;
}

int32_t sys_getsize(isr_param_t* param) {
	const char* name = (const char*)param->ebx;
	return sys_do_getsize(name);
}

int32_t sys_do_getsize(const char* name) {
	dir_entry_t e = find_file(name);
	if(e.inode_idx == 0xffffffff)return -1;
	return inode_table[e.inode_idx].size;
}

int32_t sys_listdir(isr_param_t* param) {
	dirent_t* dest = (dirent_t*)param->ebx;

	uint32_t cnt = 0;
	for(uint32_t block = 0;block < inode_table[0].block_count;++block) {
		read_sector(meta.data_start + block, FS_BUFFER);
		dir_entry_t* dirs = (dir_entry_t*)FS_BUFFER;
		for(uint32_t i = 0;i < 32;++i) {
			if(dirs[i].inode_idx == 0xffffffff)continue;
			char* filename = dest[cnt].fullname;
			strcpy(filename, dirs[i].base);
			if(dirs[i].ext[0] != '\0') {
				strcat(filename, ".");
				strcat(filename, dirs[i].ext);
			}
			dest[cnt].size = inode_table[dirs[i].inode_idx].size;
			dest[cnt].space = inode_table[dirs[i].inode_idx].block_count * 0x200;
			++cnt;
		}
	}
	return cnt;
}