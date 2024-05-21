ENTRY(main)

SECTIONS {
    . = 0xc0400000;

    .text : {
        *(.text)
    }

    . += 0x200000;
    .data : {
        *(.data)
    }

    . += 0x100000;
    .bss : {
        *(.bss)
    }
}
