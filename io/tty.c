#include "tty.h"
#include "kb.h"
#include "video.h"
#include "proc.h"
#include "syscall.h"

/* Special keys */
#define ESC			0x101	/* Esc		*/
#define TAB			0x102	/* Tab		*/
#define ENTER		0x103	/* Enter	*/
#define BACKSPACE	0x104	/* BackSpace	*/

#define GUI_L		0x105	/* L GUI	*/
#define GUI_R		0x106	/* R GUI	*/
#define APPS		0x107	/* APPS	*/

/* Shift, Ctrl, Alt */
#define SHIFT_L		0x108	/* L Shift	*/
#define SHIFT_R		0x109	/* R Shift	*/
#define CTRL_L		0x10A	/* L Ctrl	*/
#define CTRL_R		0x10B	/* R Ctrl	*/
#define ALT_L		0x10C	/* L Alt	*/
#define ALT_R		0x10D	/* R Alt	*/

/* Lock keys */
#define CAPS_LOCK	0x10E	/* Caps Lock	*/
#define	NUM_LOCK	0x10F	/* Number Lock	*/
#define SCROLL_LOCK	0x110	/* Scroll Lock	*/

/* Function keys */
#define F1			0x111	/* F1		*/
#define F2			0x112	/* F2		*/
#define F3			0x113	/* F3		*/
#define F4			0x114	/* F4		*/
#define F5			0x115	/* F5		*/
#define F6			0x116	/* F6		*/
#define F7			0x117	/* F7		*/
#define F8			0x118	/* F8		*/
#define F9			0x119	/* F9		*/
#define F10			0x11A	/* F10		*/
#define F11			0x11B	/* F11		*/
#define F12			0x11C	/* F12		*/

/* Control Pad */
#define PRINTSCREEN	0x11D	/* Print Screen	*/
#define PAUSEBREAK	0x11E	/* Pause/Break	*/
#define INSERT		0x11F	/* Insert	*/
#define DELETE		0x120	/* Delete	*/
#define HOME		0x121	/* Home		*/
#define END			0x122	/* End		*/
#define PGUP		0x123	/* Page Up	*/
#define PGDOWN		0x124	/* Page Down	*/
#define UP			0x125	/* Up		*/
#define DOWN		0x126	/* Down		*/
#define LEFT		0x127	/* Left		*/
#define RIGHT		0x128	/* Right	*/

/* ACPI keys */
#define POWER		0x129	/* Power	*/
#define SLEEP		0x12A	/* Sleep	*/
#define WAKE		0x12B	/* Wake Up	*/

/* Num Pad */
#define PAD_SLASH	0x12C	/* /		*/
#define PAD_STAR	0x12D	/* *		*/
#define PAD_MINUS	0x12E	/* -		*/
#define PAD_PLUS	0x12F	/* +		*/
#define PAD_ENTER	0x130	/* Enter	*/
#define PAD_DOT		0x131	/* .		*/
#define PAD_0		0x132	/* 0		*/
#define PAD_1		0x133	/* 1		*/
#define PAD_2		0x134	/* 2		*/
#define PAD_3		0x135	/* 3		*/
#define PAD_4		0x136	/* 4		*/
#define PAD_5		0x137	/* 5		*/
#define PAD_6		0x138	/* 6		*/
#define PAD_7		0x139	/* 7		*/
#define PAD_8		0x13A	/* 8		*/
#define PAD_9		0x13B	/* 9		*/
#define PAD_UP		PAD_8	/* Up		*/
#define PAD_DOWN	PAD_2	/* Down		*/
#define PAD_LEFT	PAD_4	/* Left		*/
#define PAD_RIGHT	PAD_6	/* Right	*/
#define PAD_HOME	PAD_7	/* Home		*/
#define PAD_END		PAD_1	/* End		*/
#define PAD_PGUP	PAD_9	/* Page Up	*/
#define PAD_PGDOWN	PAD_3	/* Page Down	*/
#define PAD_INS		PAD_0	/* Ins		*/
#define PAD_MID		PAD_5	/* Middle key	*/
#define PAD_DEL		PAD_DOT	/* Del		*/

static uint16_t keymap[0x80 * 3] = {
	/* scan-code			!Shift		Shift		E0 XX	*/
	/* =====================================================*/
	/* 0x00 - none		*/	0,			0,			0,
	/* 0x01 - ESC		*/	ESC,		ESC,		0,
	/* 0x02 - '1'		*/	'1',		'!',		0,
	/* 0x03 - '2'		*/	'2',		'@',		0,
	/* 0x04 - '3'		*/	'3',		'#',		0,
	/* 0x05 - '4'		*/	'4',		'$',		0,
	/* 0x06 - '5'		*/	'5',		'%',		0,
	/* 0x07 - '6'		*/	'6',		'^',		0,
	/* 0x08 - '7'		*/	'7',		'&',		0,
	/* 0x09 - '8'		*/	'8',		'*',		0,
	/* 0x0A - '9'		*/	'9',		'(',		0,
	/* 0x0B - '0'		*/	'0',		')',		0,
	/* 0x0C - '-'		*/	'-',		'_',		0,
	/* 0x0D - '='		*/	'=',		'+',		0,
	/* 0x0E - BS		*/	BACKSPACE,	BACKSPACE,	0,
	/* 0x0F - TAB		*/	TAB,		TAB,		0,
	/* 0x10 - 'q'		*/	'q',		'Q',		0,
	/* 0x11 - 'w'		*/	'w',		'W',		0,
	/* 0x12 - 'e'		*/	'e',		'E',		0,
	/* 0x13 - 'r'		*/	'r',		'R',		0,
	/* 0x14 - 't'		*/	't',		'T',		0,
	/* 0x15 - 'y'		*/	'y',		'Y',		0,
	/* 0x16 - 'u'		*/	'u',		'U',		0,
	/* 0x17 - 'i'		*/	'i',		'I',		0,
	/* 0x18 - 'o'		*/	'o',		'O',		0,
	/* 0x19 - 'p'		*/	'p',		'P',		0,
	/* 0x1A - '['		*/	'[',		'{',		0,
	/* 0x1B - ']'		*/	']',		'}',		0,
	/* 0x1C - CR/LF		*/	ENTER,		ENTER,		PAD_ENTER,
	/* 0x1D - l. Ctrl	*/	CTRL_L,		CTRL_L,		CTRL_R,
	/* 0x1E - 'a'		*/	'a',		'A',		0,
	/* 0x1F - 's'		*/	's',		'S',		0,
	/* 0x20 - 'd'		*/	'd',		'D',		0,
	/* 0x21 - 'f'		*/	'f',		'F',		0,
	/* 0x22 - 'g'		*/	'g',		'G',		0,
	/* 0x23 - 'h'		*/	'h',		'H',		0,
	/* 0x24 - 'j'		*/	'j',		'J',		0,
	/* 0x25 - 'k'		*/	'k',		'K',		0,
	/* 0x26 - 'l'		*/	'l',		'L',		0,
	/* 0x27 - ';'		*/	';',		':',		0,
	/* 0x28 - '\''		*/	'\'',		'"',		0,
	/* 0x29 - '`'		*/	'`',		'~',		0,
	/* 0x2A - l. SHIFT	*/	SHIFT_L,	SHIFT_L,	0,
	/* 0x2B - '\'		*/	'\\',		'|',		0,
	/* 0x2C - 'z'		*/	'z',		'Z',		0,
	/* 0x2D - 'x'		*/	'x',		'X',		0,
	/* 0x2E - 'c'		*/	'c',		'C',		0,
	/* 0x2F - 'v'		*/	'v',		'V',		0,
	/* 0x30 - 'b'		*/	'b',		'B',		0,
	/* 0x31 - 'n'		*/	'n',		'N',		0,
	/* 0x32 - 'm'		*/	'm',		'M',		0,
	/* 0x33 - ','		*/	',',		'<',		0,
	/* 0x34 - '.'		*/	'.',		'>',		0,
	/* 0x35 - '/'		*/	'/',		'?',		PAD_SLASH,
	/* 0x36 - r. SHIFT	*/	SHIFT_R,	SHIFT_R,	0,
	/* 0x37 - '*'		*/	'*',		'*',    	0,
	/* 0x38 - ALT		*/	ALT_L,		ALT_L,  	ALT_R,
	/* 0x39 - ' '		*/	' ',		' ',		0,
	/* 0x3A - CapsLock	*/	CAPS_LOCK,	CAPS_LOCK,	0,
	/* 0x3B - F1		*/	F1,			F1,			0,
	/* 0x3C - F2		*/	F2,			F2,			0,
	/* 0x3D - F3		*/	F3,			F3,			0,
	/* 0x3E - F4		*/	F4,			F4,			0,
	/* 0x3F - F5		*/	F5,			F5,			0,
	/* 0x40 - F6		*/	F6,			F6,			0,
	/* 0x41 - F7		*/	F7,			F7,			0,
	/* 0x42 - F8		*/	F8,			F8,			0,
	/* 0x43 - F9		*/	F9,			F9,			0,
	/* 0x44 - F10		*/	F10,		F10,		0,
	/* 0x45 - NumLock	*/	NUM_LOCK,	NUM_LOCK,	0,
	/* 0x46 - ScrLock	*/	SCROLL_LOCK,SCROLL_LOCK,0,
	/* 0x47 - Home		*/	PAD_HOME,	'7',		HOME,
	/* 0x48 - CurUp		*/	PAD_UP,		'8',		UP,
	/* 0x49 - PgUp		*/	PAD_PGUP,	'9',		PGUP,
	/* 0x4A - '-'		*/	PAD_MINUS,	'-',		0,
	/* 0x4B - Left		*/	PAD_LEFT,	'4',		LEFT,
	/* 0x4C - MID		*/	PAD_MID,	'5',		0,
	/* 0x4D - Right		*/	PAD_RIGHT,	'6',		RIGHT,
	/* 0x4E - '+'		*/	PAD_PLUS,	'+',		0,
	/* 0x4F - End		*/	PAD_END,	'1',		END,
	/* 0x50 - Down		*/	PAD_DOWN,	'2',		DOWN,
	/* 0x51 - PgDown	*/	PAD_PGDOWN,	'3',		PGDOWN,
	/* 0x52 - Insert	*/	PAD_INS,	'0',		INSERT,
	/* 0x53 - Delete	*/	PAD_DOT,	'.',		DELETE,
	/* 0x54 - Enter		*/	0,			0,			0,
	/* 0x55 - ???		*/	0,			0,			0,
	/* 0x56 - ???		*/	0,			0,			0,
	/* 0x57 - F11		*/	F11,		F11,		0,
	/* 0x58 - F12		*/	F12,		F12,		0,
	/* 0x59 - ???		*/	0,			0,			0,
	/* 0x5A - ???		*/	0,			0,			0,
	/* 0x5B - ???		*/	0,			0,			GUI_L,
	/* 0x5C - ???		*/	0,			0,			GUI_R,
	/* 0x5D - ???		*/	0,			0,			APPS,
	/* 0x5E - ???		*/	0,			0,			0,
	/* 0x5F - ???		*/	0,			0,			0,
	/* 0x60 - ???		*/	0,			0,			0,
	/* 0x61 - ???		*/	0,			0,			0,
	/* 0x62 - ???		*/	0,			0,			0,
	/* 0x63 - ???		*/	0,			0,			0,
	/* 0x64 - ???		*/	0,			0,			0,
	/* 0x65 - ???		*/	0,			0,			0,
	/* 0x66 - ???		*/	0,			0,			0,
	/* 0x67 - ???		*/	0,			0,			0,
	/* 0x68 - ???		*/	0,			0,			0,
	/* 0x69 - ???		*/	0,			0,			0,
	/* 0x6A - ???		*/	0,			0,			0,
	/* 0x6B - ???		*/	0,			0,			0,
	/* 0x6C - ???		*/	0,			0,			0,
	/* 0x6D - ???		*/	0,			0,			0,
	/* 0x6E - ???		*/	0,			0,			0,
	/* 0x6F - ???		*/	0,			0,			0,
	/* 0x70 - ???		*/	0,			0,			0,
	/* 0x71 - ???		*/	0,			0,			0,
	/* 0x72 - ???		*/	0,			0,			0,
	/* 0x73 - ???		*/	0,			0,			0,
	/* 0x74 - ???		*/	0,			0,			0,
	/* 0x75 - ???		*/	0,			0,			0,
	/* 0x76 - ???		*/	0,			0,			0,
	/* 0x77 - ???		*/	0,			0,			0,
	/* 0x78 - ???		*/	0,			0,			0,
	/* 0x78 - ???		*/	0,			0,			0,
	/* 0x7A - ???		*/	0,			0,			0,
	/* 0x7B - ???		*/	0,			0,			0,
	/* 0x7C - ???		*/	0,			0,			0,
	/* 0x7D - ???		*/	0,			0,			0,
	/* 0x7E - ???		*/	0,			0,			0,
	/* 0x7F - ???		*/	0,			0,			0
};

//?|?|?|?|?|?|?|SCRL|NUM|CAPS|LALT|RALT|LCTRL|RCTRL|LSHIFT|RSHIFT
static uint16_t kbflag = 0b0000000000000000;

tty_t ttys[2];
uint32_t current_tty;

void init_ttys() {
	memset(ttys, 0, sizeof(ttys));

	kbflag = 0;

	uint16_t* addr = (uint16_t*)0xc00b8000;
	for(uint32_t i = 0;i < 2;++i, addr += 0x7d0) {
		tty_t* tty = &ttys[i];
		tty->addr = addr;
		tty->head = tty->tail = &tty->in[0];
	}

	sys_switch_tty(0);

	register_syscall(6, &sys_std_read);
	register_syscall(7, &sys_std_write);
}

void sys_switch_tty(uint32_t i) {
	current_tty = i;
	uint32_t bias = ((uint32_t)ttys[i].addr - 0xc00b8000) / 2;
	outb(0x3d4, 0xc);
	outb(0x3d5, bias >> 8);
	outb(0x3d4, 0xd);
	outb(0x3d5, bias & 0xff);
	sys_set_cursor_pos(i, ttys[i].pos);
}

uint8_t get_byte_from_buffer() {
	while(cnt <= 0)keyboard_handler(0);
	--cnt;
	uint8_t ret = *tail;
	if(++tail >= &buffer[32])tail = &buffer[0];
	return ret;
}

void put_tty_in_buffer(uint32_t tty, uint8_t c) {
	tty_t* p = &ttys[tty];
	if(c == '\b') {
		if(p->cnt == 0)return;
		if(--p->head < p->in)p->head = &p->in[127];
		--p->cnt;
	} else {
		if(p->cnt >= 128)return;
		(*p->head++) = c;
		if(p->head >= &p->in[128])p->head = p->in;
		++p->cnt;
	}
	sys_putchar(current_tty, c);
}

void transfer_buffer() {
	uint16_t key = get_byte_from_buffer();
	int mark;

	// interpret scancode
	if(key == 0xe0) {
		key = get_byte_from_buffer();
		if(key == 0x2a) {	//ptsc mark
			uint8_t s1 = get_byte_from_buffer(), s2 = get_byte_from_buffer();
			if(s1 == 0xe0 && s2 == 0x37)key = PRINTSCREEN, mark = 1;
		} else if(key == 0xb7) {	//ptsc break
			uint8_t s1 = get_byte_from_buffer(), s2 = get_byte_from_buffer();
			if(s1 == 0xe0 && s2 == 0xaa)key = PRINTSCREEN, mark = 0;
		} else {	//other e0 extended codes
			mark = key & 0x80 ? 0 : 1;
			key &= 0x7f;
			key = keymap[key * 3 + 2];
		}
	} else if(key == 0xe1) {	//pause
		uint8_t s[5];
		for(uint32_t i = 0;i < 5;++i)s[i] = get_byte_from_buffer();
		if(s[0] == 0x1d && s[1] == 0x45 && s[2] == 0xe1 && s[3] == 0x9d && s[4] == 0xc5)key = PAUSEBREAK, mark = 1;
	} else {	//normal codes
		mark = key & 0x80 ? 0 : 1;
		key &= 0x7f;
		if(kbflag & 0b11)key = keymap[key * 3 + 1];
		else key = keymap[key * 3];
	}

	// interpret key
	switch(key) {
	case UP:
	case DOWN:
	case LEFT:
	case RIGHT:
		break;
	case F1:
	case F2:
	case F3:
	case F4:
	case F5:
	case F6:
	case F7:
	case F8:
	case F9:
	case F10:
	case F11:
	case F12:
		if(mark && (kbflag & 0b1100) && (kbflag & 0b110000) && key - F1 < 2) {
			sys_switch_tty(key - F1);
		}
		break;
	case HOME:
	case END:
	case PGUP:
	case PGDOWN:
	case INSERT:
	case DELETE:
	case ESC:
	case PAUSEBREAK:
	case PRINTSCREEN:
		break;
	case GUI_L:
	case GUI_R:
	case APPS:
		break;
	case POWER:
	case SLEEP:
	case WAKE:
		break;
	case PAD_SLASH:
	case PAD_STAR:
	case PAD_MINUS:
	case PAD_PLUS:
	case PAD_ENTER:
	case PAD_DOT:
	case PAD_0:
	case PAD_1:
	case PAD_2:
	case PAD_3:
	case PAD_4:
	case PAD_5:
	case PAD_6:
	case PAD_7:
	case PAD_8:
	case PAD_9:
		break;
	case SHIFT_R:
		if(mark)kbflag |= (1 << 0);
		else kbflag &= ~(1 << 0);
		break;
	case SHIFT_L:
		if(mark)kbflag |= (1 << 1);
		else kbflag &= ~(1 << 1);
		break;
	case CTRL_R:
		if(mark)kbflag |= (1 << 2);
		else kbflag &= ~(1 << 2);
		break;
	case CTRL_L:
		if(mark)kbflag |= (1 << 3);
		else kbflag &= ~(1 << 3);
		break;
	case ALT_R:
		if(mark)kbflag |= (1 << 4);
		else kbflag &= ~(1 << 4);
		break;
	case ALT_L:
		if(mark)kbflag |= (1 << 5);
		else kbflag &= ~(1 << 5);
		break;
	case CAPS_LOCK:
		if(mark)kbflag ^= (1 << 6);
		break;
	case NUM_LOCK:
		if(mark)kbflag ^= (1 << 7);
		break;
	case SCROLL_LOCK:
		if(mark)kbflag ^= (1 << 8);
		break;
	case TAB:
		// if(mark)put_tty_in_buffer(current_tty, '\t');
		break;
	case BACKSPACE:
		if(mark)put_tty_in_buffer(current_tty, '\b');
		break;
	case ENTER:
		if(mark) {
			put_tty_in_buffer(current_tty, '\n');
			for(uint32_t i = 0;i < PROC_MAX_CNT;++i)if(proc_table[i]->stat == WAITING_INPUT && proc_table[i]->tty == current_tty) {
				proc_table[i]->stat = READY;
			}
		}
		break;
	default:
		if(mark) {
			if(kbflag & (1 << 6)) {
				if(key >= 'a' && key <= 'z')key -= 32;
				else if(key >= 'A' && key <= 'Z')key += 32;
			}
			put_tty_in_buffer(current_tty, key);
		}
		break;
	}
}

void tty_driver() {
	while(1) {
		transfer_buffer();
	}
}

int32_t sys_std_read(isr_param_t* param) {
	char* dest = (char*)param->ebx;
	uint32_t max_size = (uint32_t)param->ecx;

	proc_table[current_proc]->stat = WAITING_INPUT;
	asm volatile("int $0x20");

	tty_t* tty = &ttys[proc_table[current_proc]->tty];
	uint32_t i = 0;
	while(tty->cnt > 0 && (*tty->tail) != '\n' && i < max_size) {
		dest[i++] = (*tty->tail++);
		if(tty->tail == &tty->in[128])tty->tail = &tty->in[0];
		--tty->cnt;
	}
	while(tty->cnt > 0)++tty->tail, --tty->cnt;

	return i;
}

int32_t sys_std_write(isr_param_t* param) {
	const char* data = (const char*)param->ebx;

	sys_puts(proc_table[current_proc]->tty, data);

	return 0;
}
