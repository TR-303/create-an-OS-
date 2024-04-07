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

CODE32_START        equ 0
CODE32_LIMIT        equ 0xfffff
CODE32_ATTR         equ 0b0100000010011000

DATA_START          equ 0
DATA_LIMIT          equ 0xfffff
DATA_ATTR           equ 0b0000000010010010

VIDEO_START         equ 0xb8000
VIDEO_LIMIT         equ 0x7fff
VIDEO_ATTR          equ 0b0000000010010010