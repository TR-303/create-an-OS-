#include "proc.h"
#include "int.h"
#include "tty.h"
#include "syscall.h"
#include "paging.h"
#include "video.h"
#include "user/interface.h"

pcb_t* proc_table[PROC_MAX_CNT];
uint32_t current_proc;

void init_proc() {
	current_proc = -1;
	for(int i = 0, addr = 0xc0500000 + 0x1000 - sizeof(pcb_t); i < PROC_MAX_CNT; ++i, addr += 0x1000) {
		proc_table[i] = (pcb_t*)addr;
		proc_table[i]->stat = EMPTY;
	}
	register_syscall(8, &sys_fork);
	register_syscall(9, &sys_exit);
	register_syscall(10, &sys_wait);
	register_syscall(11, &sys_exec);
}

void sys_setup_kernel_process(uint32_t pid, void* func) {
	pcb_t* pcb = proc_table[pid];
	memset(pcb, 0, sizeof(pcb_t));
	pcb->pid = pid;
	pcb->privilege = 0;
	pcb->father = 0;
	pcb->stat = READY;
	pcb->tty = current_tty;
	pcb->pde = PAGE_TABLE_START;
	pcb->kstktop = (uint32_t)pcb;
	pcb->regs.cs = SELECTOR_KERNEL_CODE;
	pcb->regs.ds = pcb->regs.es = pcb->regs.fs = pcb->regs.gs = SELECTOR_KERNEL_DATA;
	pcb->regs.ss = SELECTOR_KERNEL_DATA;
	pcb->regs.eip = (uint32_t)func;
	pcb->regs.eflags = 0x00000202;
}

int32_t sys_fork(isr_param_t* param) {
	uint32_t newpid = 0;
	while(newpid < PROC_MAX_CNT && proc_table[newpid]->stat != EMPTY) ++newpid;
	if(newpid == PROC_MAX_CNT)return -1;
	memcpy((void*)(PROC_KSTACK_TABLE_START + 0x1000 * newpid), (void*)(PROC_KSTACK_TABLE_START + 0x1000 * current_proc), 0x1000);

	pcb_t* pcb = proc_table[newpid];
	pcb->pid = newpid;
	pcb->father = current_proc;
	sys_copy_pages(proc_table[current_proc], pcb);

	isr_param_t* new_param = (isr_param_t*)((uint32_t)param + 0x1000 * (newpid - current_proc));
	isr_param_t* newesp = (isr_param_t*)((uint32_t)new_param - sizeof(isr_param_t) + 8);
	isr_param_t stk = { SELECTOR_KERNEL_DATA,SELECTOR_KERNEL_DATA,SELECTOR_KERNEL_DATA,SELECTOR_KERNEL_DATA,0,0,0,0,0,0,0,0,0,0,(uint32_t)&syscall_resume,SELECTOR_KERNEL_CODE,0x202 };
	memcpy(newesp, &stk, sizeof(isr_param_t) - 8);
	pcb->kstktop = (uint32_t)newesp;
	if(pcb->privilege == 0)new_param->ebp += 0x1000 * (newpid - current_proc);

	return newpid;
}

int32_t sys_exit(isr_param_t* param) {
	pcb_t* pcb = proc_table[current_proc];
	pcb_t* fpcb = proc_table[pcb->father];

	for(uint32_t i = 0;i < 4;++i)if(pcb->fds[i] != 0)sys_do_close(i);

	for(uint32_t i = 0;i < PROC_MAX_CNT;++i) {
		if(proc_table[i]->father == current_proc && i != current_proc)proc_table[i]->father = 0;
	}

	sys_cleanup_memory(current_proc);

	while(fpcb->stat != WAITING) {
		pcb->stat = HANGING;
		asm volatile("int $0x20");
	}
	fpcb->stat = READY;
	isr_param_t* stk = (isr_param_t*)fpcb->kstktop;
	stk->eax = current_proc;
	pcb->stat = HANGING;
	asm volatile("int $0x20");
	while(1);
}

int32_t sys_wait(isr_param_t* param) {
	pcb_t* pcb = proc_table[current_proc];
	while(1) {
		uint32_t f = 0;
		for(uint32_t i = 0;i < PROC_MAX_CNT;++i) {
			if(proc_table[i]->father == current_proc && i != current_proc) {
				f = 1;
				if(proc_table[i]->stat == HANGING) {
					proc_table[i]->stat = DEAD;
					return proc_table[i]->pid;
				}
			}
		}
		if(f == 0)return -1;
		pcb->stat = WAITING;
		asm volatile("int $0x20");
	}
}

int32_t sys_exec(isr_param_t* param) {
	char file[16];
	strcpy(file, (const char*)param->ebx);
	int32_t size = sys_do_getsize(file);
	if(size == -1)return -1;
	if(size > 0x80000)while(1);

	uint32_t argc = param->ecx;
	char** argv = (char**)param->edx;
	char* nargv[16];

	uint8_t user_stack[0x100];
	int32_t p = 0xff;

	for(uint32_t i = 0;i < argc;++i) {
		p -= strlen(argv[i]) + 1;
		if(p < 0)while(1);
		strcpy(&user_stack[p], argv[i]);
		nargv[i] = (uint8_t*)(0xb0000000 - (uint32_t)&user_stack[0x100] + (uint32_t)&user_stack[p]);
	}
	while(p & 0b11)--p;
	p -= argc * 4;
	if(p < 0)while(1);
	memcpy(&user_stack[p], nargv, argc * 4);

	sys_cleanup_memory(current_proc);

	int32_t fp = sys_do_open(file, OPEN_EXIST);
	sys_do_read(fp, EXEC_BUFFER, size);
	sys_do_close(fp);
	Elf32_Ehdr* elfhdr = (Elf32_Ehdr*)EXEC_BUFFER;
	for(uint32_t i = 0;i < elfhdr->e_phnum;++i) {
		Elf32_Phdr* proghdr = (Elf32_Phdr*)(EXEC_BUFFER + elfhdr->e_phoff + (i * elfhdr->e_phentsize));
		if(proghdr->p_type == 1)memcpy((void*)proghdr->p_vaddr, EXEC_BUFFER + proghdr->p_offset, proghdr->p_filesz);
	}

	pcb_t* pcb = proc_table[current_proc];
	pcb->regs.ss = SELECTOR_USER_DATA;
	pcb->regs.esp = 0xb0000000 - (uint32_t)&user_stack[0x100] + (uint32_t)&user_stack[p];
	memcpy((uint8_t*)(0xb0000000 - 0x100), user_stack, 0x100);
	pcb->regs.ecx = argc;
	pcb->regs.eip = elfhdr->e_entry;
	pcb->regs.cs = SELECTOR_USER_CODE;
	pcb->regs.ds = pcb->regs.es = pcb->regs.fs = pcb->regs.gs = SELECTOR_USER_DATA;
	pcb->regs.eflags = 0x3202;
	pcb->kstktop = (uint32_t)pcb;
	pcb->privilege = 3;
	asm volatile("mov %0, %%eax"::"m"(pcb->regs.esp));

	exec_start();
}

void setup_init() {
	sys_setup_kernel_process(0, &init_start);
	sys_create_proc_page_table(0);
}

void init_start() {
	int pid = fork();
	if(pid == 0)exec("shell.elf", 0, 0);
	pid = fork();
	if(pid == 0) {
		proc_table[current_proc]->tty = 1;
		exec("shell.elf", 0, 0);
	}
	exec("init.elf", 0, 0);
	while(1);
}