#include "paging.h"
#include "video.h"
#include "tty.h"
#include "alloc.h"

static uint8_t page_bitmap[(0xc0600000 - PAGE_TABLE_START) / 0x1000 / 8];

static uint32_t frame_copy_map[128][2];

static void add(uint32_t key) {
	for(uint32_t i = 0;i < 128;++i)if(frame_copy_map[i][0] == key) {
		++frame_copy_map[i][1];
		return;
	}
	for(uint32_t i = 0;i < 128;++i)if(frame_copy_map[i][0] == 0) {
		frame_copy_map[i][0] = key;
		frame_copy_map[i][1] = 2;
		return;
	}
	while(1);
}

static uint32_t dec(uint32_t key) {
	for(uint32_t i = 0;i < 64;++i)if(frame_copy_map[i][0] == key) {
		if(--frame_copy_map[i][1] == 0)frame_copy_map[i][0] = 0;
		return frame_copy_map[i][1];
	}
	return 0;
}

static void flushTLB() {
	asm volatile (
		"movl %%cr3, %%eax\n\t"
		"movl %%eax, %%cr3\n\t"
		::: "eax");
}

uint32_t page_vir_to_phy(uint32_t addr) {
	return addr - 0xc0300000;
}
uint32_t page_phy_to_vir(uint32_t addr) {
	return addr + 0xc0300000;
}

void init_paging() {
	register_isr_handler(14, &page_fault_handler);
	memset(page_bitmap, 0, sizeof(page_bitmap));
	memset(frame_copy_map, 0, sizeof(frame_copy_map));
	uint32_t* base_pde = (uint32_t*)PAGE_TABLE_START;
	memset(base_pde, 0, 0x3000);
	set_bit(page_bitmap, 0, 1);
	set_bit(page_bitmap, 1, 1);
	set_bit(page_bitmap, 2, 1);
	base_pde[768] = page_vir_to_phy(PAGE_TABLE_START + 0x1000) | 0b11;
	base_pde[769] = page_vir_to_phy(PAGE_TABLE_START + 0x2000) | 0b11;
	uint32_t* base_pte = (uint32_t*)(PAGE_TABLE_START + 0x1000);
	for(uint32_t i = 0, addr = 0;i < 256;++i, addr += 0x1000) base_pte[i] = addr | 0b11;
	base_pte = (uint32_t*)(PAGE_TABLE_START + 0x2000);
	for(uint32_t i = 0, addr = 0x100000;i < 512;++i, addr += 0x1000)base_pte[i] = addr | 0b11;
	asm volatile("add $0xc0000000, %esp");
	asm volatile("add $0xc0000000, %ebp");
	asm volatile("mov %0, %%cr3"::"r"(page_vir_to_phy(PAGE_TABLE_START)));
}

uint32_t sys_alloc_page_frame() {
	for(uint32_t i = 0;i < (0xc0600000 - PAGE_TABLE_START) / 0x1000;++i)if(get_bit(page_bitmap, i) == 0) {
		set_bit(page_bitmap, i, 1);
		uint32_t* ret = (uint32_t*)(PAGE_TABLE_START + i * 0x1000);
		memset(ret, 0, 0x1000);
		return page_vir_to_phy((uint32_t)ret);
	}
	while(1);
}

void sys_free_page_frame(uint32_t addr) {
	set_bit(page_bitmap, (addr - PAGE_TABLE_START) / 0x1000, 0);
}

void page_fault_handler(isr_param_t* param) {
	uint32_t vir_addr;
	asm volatile("mov %%cr2, %0":"=r"(vir_addr));

	uint32_t present = param->err_code & 0x1;
	uint32_t rw = param->err_code & 0x2;
	uint32_t user_mode = param->err_code & 0x4;

	if(!present) {
		uint32_t* pde = (uint32_t*)proc_table[current_proc]->pde;
		uint32_t pdeidx = vir_addr >> 22;
		if((pde[pdeidx] & 1) == 0)pde[pdeidx] = sys_alloc_page_frame() | 0b111;
		uint32_t* pte = (uint32_t*)page_phy_to_vir(pde[pdeidx] & 0xfffff000);
		uint32_t pteidx = (vir_addr >> 12) & 0x3ff;
		pte[pteidx] = sys_alloc_phy_page() | 0b111;
	} else if(rw) {
		uint32_t* pde = (uint32_t*)proc_table[current_proc]->pde;
		uint32_t pdeidx = vir_addr >> 22;
		uint32_t* pte = (uint32_t*)page_phy_to_vir(pde[pdeidx] & 0xfffff000);
		uint32_t pteidx = (vir_addr >> 12) & 0x3ff;
		if(dec(pte[pteidx] & 0xfffff000)) {
			memcpy(PAGE_BUFFER, (void*)(vir_addr & 0xfffff000), 0x1000);
			pte[pteidx] = sys_alloc_phy_page() | 0b111;
			memcpy((void*)(vir_addr & 0xfffff000), PAGE_BUFFER, 0x1000);
		} else pte[pteidx] |= 0b10;
	}
	flushTLB();
}

void sys_copy_pages(pcb_t* src, pcb_t* dest) {
	uint32_t* spde = (uint32_t*)src->pde;
	dest->pde = page_phy_to_vir(sys_alloc_page_frame());
	uint32_t* dpde = (uint32_t*)dest->pde;
	for(uint32_t i = 0;i < 768;++i) {
		if(spde[i] & 1) {
			dpde[i] = sys_alloc_page_frame() | 0b111;
			uint32_t* dpte = (uint32_t*)page_phy_to_vir(dpde[i] & 0xfffff000);
			uint32_t* spte = (uint32_t*)page_phy_to_vir(spde[i] & 0xfffff000);
			for(uint32_t j = 0;j < 1024;++j)if(spte[j] & 1) {
				spte[j] &= 0xfffffffd;
				add(spte[j] & 0xfffff000);
			}
			memcpy(dpte, spte, 0x1000);
		} else dpde[i] = 0;
	}
	memcpy(dpde + 768, spde + 768, 0x1000 / 4);
	flushTLB();
}

void sys_cleanup_memory(uint32_t pid) {
	uint32_t* pde = (uint32_t*)proc_table[pid]->pde;
	for(uint32_t i = 0;i < 768;++i) {
		if(pde[i] & 1) {
			uint32_t* pte = (uint32_t*)page_phy_to_vir(pde[i] & 0xfffff000);
			for(uint32_t j = 0;j < 1024;++j) {
				if(pte[j] & 1) {
					if(!dec(pte[j] & 0xfffff000))sys_free_phy_page(pte[j] & 0xfffff000);
				}
			}
			sys_free_page_frame((uint32_t)pte);
			pde[i] = 0;
		}
	}
	flushTLB();
}

void sys_create_proc_page_table(uint32_t pid) {
	pcb_t* pcb = proc_table[pid];
	pcb->pde = page_phy_to_vir(sys_alloc_page_frame());

	uint32_t* pde = (uint32_t*)pcb->pde;
	uint32_t* base_pde = (uint32_t*)PAGE_TABLE_START;
	memcpy(pde + 768, base_pde + 768, 0x1000 / 4);
}