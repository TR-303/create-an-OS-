#include "common.h"

struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint16_t attributes;
    uint8_t base_high;
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

void set_gdt_entry(int num, uint32_t base, uint32_t limit, uint16_t attributes);

void reload_gdt();

void set_tss(uint32_t esp0);
