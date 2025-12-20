#include <tty.h>
#define TERMINAL_COLOR 0x07
static int x, y;
static const unsigned char rows, cols, depth;
static const unsigned char rows = 80, cols = 25, depth = 2;

void tty_clearline(unsigned char from, unsigned char to) {
	unsigned short i = rows * from * depth;
	char* vidmem = (char*) 0xb8000;
	for (i; i < (rows * to * depth); i++) {
		vidmem[(i / 2)*2] = 0;
		vidmem[(i / 2)*2 + 1] = TERMINAL_COLOR;
	}
}

void tty_clean() {
	tty_clearline(0, cols-1);
	x = 0, y = 0;
	tty_refresh();
}

void tty_scroll(unsigned char lines) {
	char* vidmem = (char*) 0xb8000;
	unsigned short i = 0;
	tty_clearline(0, lines-1);
	for (i; i < rows * (cols-1) * 2; i++) {
		vidmem[i] = vidmem[i + rows * 2 * lines ];
	}
	tty_clearline(cols-1-lines, cols-1);
	if ((y - lines) < 0) {
		x = 0;
		y = 0;
	} else {
		y -= lines;
	}

	tty_refresh();
}

void tty_checknl() {
	if (y >= cols-1) {
		tty_scroll(1);
	}
}

void tty_refresh() {
	unsigned tmp = y * rows + x;

	outb(0x3d4, 14);
	outb(0x3d5, tmp >> 8);
	outb(0x3d4, 15);
	outb(0x3d5, tmp);
}

void tty_putch(char c) {
	char* vidmem = (char*) 0xb8000;

	switch(c) {
		case ('\n'):
			x = 0;
			y++;
			break;
		case(0x08):
			if (x > 0) {
				x--;
				vidmem[(y * rows + x)*depth] = 0;
			}
			break;

		default:
			vidmem[((y * rows + x))*depth] = c;
			vidmem[((y * rows + x))*depth+1] = TERMINAL_COLOR;
			x++;
	}

	if (x >= rows) {
		x = 0;
		y++;
	}
	
	tty_checknl();
	tty_refresh();
}

void tty_cursor(unsigned char c_start, unsigned char c_end) {
	outb(0x3d4, 0x0a);
	outb(0x3d5, (inb(0x3d5) & 0xc0) | c_start);
	outb(0x3d4, 0x0b);
	outb(0x3d5, (inb(0x3d5) & 0xe0) | c_end);
}