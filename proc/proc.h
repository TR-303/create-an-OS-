#pragma once
#include "common.h"
#include "int.h"
#include "fs.h"
#define PROC_KSTACK_TABLE_START 0xc0500000
#define PROC_MAX_CNT 32

#define EXEC_BUFFER (uint8_t*)0xc0420000

typedef struct interrupt_stack_frame proc_stk_t;

enum STATUS {
	EMPTY = 0,
	READY,
	WAITING_INPUT,
	WAITING,
	HANGING,
	DEAD
};
typedef uint32_t STATUS;

typedef struct pcb {
	proc_stk_t regs;
	uint32_t kstktop;
	file_desc_t* fds[4];
	uint32_t pde;
	uint32_t tty;
	STATUS stat;
	uint32_t father;
	uint32_t privilege;
	uint32_t pid;
}pcb_t;

extern pcb_t* proc_table[PROC_MAX_CNT];
extern uint32_t current_proc;

typedef struct {
	uint8_t e_ident[16];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} Elf32_Ehdr;

typedef struct {
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_vaddr;
	uint32_t p_paddr;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
	uint32_t p_align;
} Elf32_Phdr;

void init_proc();

void sys_setup_kernel_process(uint32_t pid, void* func);

int32_t sys_fork(isr_param_t* param);

int32_t sys_exit(isr_param_t* param);

int32_t sys_wait(isr_param_t* param);

int32_t sys_exec(isr_param_t* param);

extern void syscall_resume();

extern void exec_start();

void setup_init();

void init_start();