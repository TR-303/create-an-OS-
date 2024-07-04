;constants
;should not produce any bit after nasm

MBR_START_ADDR      equ 0x7c00
LOADER_START_ADDR   equ 0x7e00

;the function shared by mbr and loader, its addr stored there
READ_FAT12_FILE_FUNC_ADDR_ADDR equ 0x7dfc

%macro Descriptor 3
    dw %2 & 0xffff
    dw %1 & 0xffff
    db (%1 >> 16) & 0xff
    dw ((%2 >> 8) & 0xf00) | (%3 & 0xf0ff)
    db (%1 >>24) & 0xff
%endmacro

ATTR_LDCODE32       equ 0b1100000010011000
ATTR_DATA           equ 0b1000000010010010
ATTR_VIDEO          equ 0b0000000010010010

LDCODE32_START      equ 0
DATA_START          equ 0
VEDIO_START         equ 0xb8000

PAGE_TABLE_SIZE         equ 0x1000  ;4kb
PAGE_SIZE               equ 0x1000  ;4kb

KERNEL_PDE_START        equ 0x1000
KERNEL_PTE_START        equ 0x2000

KERNEL_ELF_PHI_START    equ 0x9000
KERNEL_PHY_START        equ 0x100000
KERNEL_VIR_START        equ 0xc0400000