#include "video.h"
#include "kb.h"
#include "tty.h"

void sys_set_cursor_pos(uint32_t tty_num, uint16_t position) {
    if(tty_num == current_tty) {
        uint32_t true_position = ((uint32_t)ttys[current_tty].addr - 0xc00b8000) / 2 + position;
        outb(0x3d4, 0xe);
        outb(0x3d5, true_position >> 8);

        outb(0x3d4, 0xf);
        outb(0x3d5, true_position & 0xff);
    }

    ttys[tty_num].pos = position;
}

void sys_set_cursor_coord(uint32_t tty_num, uint8_t x, uint8_t y) {
    uint16_t position = y * SCREEN_WIDTH + x;
    ttys[tty_num].pos = position;
    sys_set_cursor_pos(tty_num, position);
}

uint16_t get_cursor_pos(uint32_t tty_num) {
    return ttys[tty_num].pos;
}

void sys_roll_screen(uint32_t tty_num) {
    uint16_t* video_memory = ttys[tty_num].addr;
    for(int i = 0; i < SCREEN_SIZE - SCREEN_WIDTH; ++i)
        video_memory[i] = video_memory[i + SCREEN_WIDTH];
    for(int i = SCREEN_SIZE - SCREEN_WIDTH; i < SCREEN_SIZE; ++i)
        video_memory[i] = 0x20 | (COLOR_DEFAULT << 8);
}

void sys_putchar_with_color(uint32_t tty_num, uint8_t c, uint8_t color) {
    uint16_t pos = get_cursor_pos(tty_num);
    uint16_t* video_memory = ttys[tty_num].addr;

    if(c == '\n')pos = (pos / SCREEN_WIDTH + 1) * SCREEN_WIDTH;
    else if(c == '\t')pos = (pos / 8 + 1) * 8;
    else if(c == '\b') {
        if(pos > 0)video_memory[--pos] = (0x20 | (COLOR_DEFAULT << 8));
    } else video_memory[pos++] = (c | (color << 8));
    while(pos >= ((uint32_t)video_memory - 0xc00b8000) / 2 + SCREEN_SIZE) {
        sys_roll_screen(tty_num);
        pos -= SCREEN_WIDTH;
    }
    sys_set_cursor_pos(tty_num, pos);
}

void sys_putchar(uint32_t tty_num, uint8_t c) {
    sys_putchar_with_color(tty_num, c, COLOR_DEFAULT);
}

void sys_puts_with_color(uint32_t tty_num, const char* s, uint8_t color) {
    for(int i = 0; s[i] != '\0'; ++i) {
        sys_putchar_with_color(tty_num, s[i], color);
    }
}

void sys_puts(uint32_t tty_num, const char* s) {
    sys_puts_with_color(tty_num, s, COLOR_DEFAULT);
}

void sys_clear_screen(uint32_t tty_num) {
    uint16_t* video_memory = ttys[tty_num].addr;
    uint16_t blank = (0x20 | (COLOR_DEFAULT << 8));
    for(int i = 0; i < SCREEN_SIZE; ++i) {
        video_memory[i] = blank;
    }
    sys_set_cursor_pos(tty_num, 0);
}