ENTRY(_start)

SECTIONS {
    . = 0xc0400000;
    .text : {*(.text)}
    . = . + 0x10000;
    .data : {*(.data)}
    . = . + 0x10000;
    .bss : {*(.bss)}
}