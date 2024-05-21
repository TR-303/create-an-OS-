#include "video.h"

uint16_t *video_memory = (uint16_t *)VEDIO_START_ADDR;

void set_cursor_pos(uint16_t position)
{
    outb(0x3d4, 0xe);
    outb(0x3d5, position >> 8);

    outb(0x3d4, 0xf);
    outb(0x3d5, position & 0xff);
}

void set_cursor_coord(uint8_t x, uint8_t y)
{
    uint16_t position = (uint16_t)x * 80 + y;
    set_cursor_pos(position);
}

uint16_t get_cursor_pos()
{
    uint8_t h, l;

    outb(0x3d4, 0xe);
    h = inb(0x3d5);

    outb(0x3d4, 0xf);
    l = inb(0x3d5);

    return (h << 8) | l;
}

void roll_screen()
{
    for (int i = 0; i < 1920; ++i)
        video_memory[i] = video_memory[i + 80];
    for (int i = 1920; i < 2000; ++i)
        video_memory[i] = 0x20 | (COLOR_DEFAULT << 8);
}

void putchar_with_color(uint8_t c, uint8_t color)
{
    uint16_t pos = get_cursor_pos();
    if (c == '\n')
        pos = (pos / 80 + 1) * 80;
    else if (c == '\t')
        pos = (pos / 8 + 1) * 8;
    else if (c == 0x08) // backspace
        video_memory[--pos] = (0x20 | (color << 8));
    else
        video_memory[pos++] = (c | (color << 8));
    if (pos >= 2000)
    {
        roll_screen();
        pos -= 80;
    }
    set_cursor_pos(pos);
}

void putchar(uint8_t c)
{
    putchar_with_color(c, COLOR_DEFAULT);
}

void puts_with_color(char *s, uint8_t color)
{
    for (int i = 0; s[i] != '\0'; ++i)
    {
        putchar_with_color(s[i], color);
    }
}

void puts(char *s)
{
    puts_with_color(s, COLOR_DEFAULT);
}

void clear_screen()
{
    int16_t blank = (0x20 | (COLOR_DEFAULT << 8));
    for (int i = 0; i < 2000; ++i)
    {
        video_memory[i] = blank;
    }
}