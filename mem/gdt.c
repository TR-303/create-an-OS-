#include "gdt.h"
#include "video.h"
#include "tty.h"

static gdt_entry_t gdt[8];
static gdt_ptr_t gdt_ptr;

static tss_t tss;

void reload_gdt() {
    memset(gdt, 0, sizeof(gdt));
    // empty
    set_gdt_entry(0, 0, 0, 0);
    // kernel code(DPL 0)
    set_gdt_entry(1, 0, 0xfffff, 0b1100000010011000);
    // kernel data(DPL 0)
    set_gdt_entry(2, 0, 0xfffff, 0b1100000010010010);
    // user code(DPL 3)
    set_gdt_entry(3, 0, 0xbffff, 0b1100000011111000);
    // user data(DPL 3)
    set_gdt_entry(4, 0, 0xbffff, 0b1100000011110010);

    gdt_ptr.base = (uint32_t)gdt;
    gdt_ptr.limit = sizeof(gdt) - 1;

    asm volatile("lgdt [%0]":: "m"(gdt_ptr));

    init_tss();

    asm volatile(
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        ::: "ax");

    return;
}

void set_gdt_entry(int num, uint32_t base, uint32_t limit, uint16_t attributes) {
    gdt[num].limit_low = limit & 0xffff;
    gdt[num].base_low = base & 0xffff;
    gdt[num].base_middle = (base >> 16) & 0xff;
    gdt[num].attributes = attributes | (limit >> 16 << 8);
    gdt[num].base_high = base >> 24;
}

void init_tss() {
    memset(&tss, 0, sizeof(tss));
    tss.ss0 = SELECTOR_KERNEL_DATA;
    tss.iomap_base = 0xffff;
    set_gdt_entry(5, (uint32_t)&tss, sizeof(tss) - 1, 0b0100000010001001);
    asm volatile("ltr %%ax":: "a"(SELECTOR_TSS));
}

void set_tss(uint32_t esp0) {
    tss.esp0 = esp0;
}