;constants

MBR_START_ADDR      equ 0x7c00
LOADER_START_ADDR   equ 0x7e00

%macro Descriptor 3
    dw %2 & 0xffff
    dw %1 & 0xffff
    db (%1 >> 16) & 0xff
    dw ((%2 >> 8) & 0xf00) | (%3 & 0xf0ff)
    db (%1 >>24) & 0xff
%endmacro

ATTR_EXEC         equ 0b0100000010011000
ATTR_DATA         equ 0b0000000010010010

PAGE_TABLE_SIZE         equ 0x1000  ;4kb
PAGE_SIZE               equ 0x1000  ;4kb

PAGE_START              equ 0x100000
PAGE_LIMIT              equ 0xfffff
PAGE_ATTR               equ 0b0000000010010010

;based at PAGE_START
PAGE_DIR_PHY_ADDR       equ 0xff000
PAGE_TABLE_PHY_START    equ 0
;

PAGE_DIR_VIR_ADDR       equ 0xfffff000
PAGE_TABLE_VIR_START    equ 0xfff00000

KERNEL_ELF_VIR_START    equ 0xc0100000