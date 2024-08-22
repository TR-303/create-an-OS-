#pragma once
#include "common.h"
#define USER_SPACE_START 0x300000

void init_memory();

uint32_t sys_alloc_phy_page();

void sys_free_phy_page(uint32_t addr);