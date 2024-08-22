#include "alloc.h"

//29MB
static uint8_t mem_bitmap[0x1d00 / 8];
static uint32_t mp;

void init_memory() {
	memset(mem_bitmap, 0, sizeof(mem_bitmap));
	mp = 0;
}

uint32_t sys_alloc_phy_page() {
	for(uint32_t i = 0;i < 0x1d00 / 8;++i, ++mp) {
		if(get_bit(mem_bitmap, mp) == 0) {
			set_bit(mem_bitmap, mp, 1);
			return USER_SPACE_START + 0x1000 * mp;
		}
	}
	while(1);
}

void sys_free_phy_page(uint32_t addr) {
	set_bit(mem_bitmap, addr - (USER_SPACE_START) / 0x1000, 0);
}
