#pragma once
#include "common.h"
#include "int.h"

#define ESC 0x01
#define BACKSPACE 0x0E
#define TAB 0x0F
#define ENTER 0x1C
#define CTRL_L 0x1D
#define SHIFT_L 0x2A
#define SHIFT_R 0x36
#define ALT_L 0x38
#define CAPS_LOCK 0x3A
#define F1 0x3B
#define F2 0x3C
#define F3 0x3D
#define F4 0x3E
#define F5 0x3F
#define F6 0x40
#define F7 0x41
#define F8 0x42
#define F9 0x43
#define F10 0x44
#define NUM_LOCK 0x45
#define SCROLL_LOCK 0x46
#define PAD_HOME 0x47
#define HOME 0x47 // same as PAD_HOME
#define PAD_UP 0x48
#define UP 0x48 // same as PAD_UP
#define PAD_PAGEUP 0x49
#define PAGEUP 0x49 // same as PAD_PAGEUP
#define PAD_MINUS 0x4A
#define PAD_LEFT 0x4B
#define LEFT 0x4B // same as PAD_LEFT
#define PAD_MID 0x4C
#define PAD_RIGHT 0x4D
#define RIGHT 0x4D // same as PAD_RIGHT
#define PAD_PLUS 0x4E
#define PAD_END 0x4F
#define END 0x4F // same as PAD_END
#define PAD_DOWN 0x50
#define DOWN 0x50 // same as PAD_DOWN
#define PAD_PAGEDOWN 0x51
#define PAGEDOWN 0x51 // same as PAD_PAGEDOWN
#define PAD_INS 0x52
#define INSERT 0x52 // same as PAD_INS
#define PAD_DOT 0x53
#define DELETE 0x53 // same as PAD_DOT
#define F11 0x57
#define F12 0x58
#define GUI_L 0x5B // Left Windows key
#define GUI_R 0x5C // Right Windows key
#define APPS 0x5D  // Applications key (Menu key)
#define PAD_SLASH 0x35
#define CTRL_R 0x1D    // same as CTRL_L for now
#define ALT_R 0x38     // same as ALT_L for now
#define PAD_ENTER 0x1C // same as ENTER for now

void init_keyboard_interrupt();

void enable_keyboard_interrupt();

void keyboard_handler(isr_param_t param);

char read_buffer();