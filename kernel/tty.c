/*
 * Teletype writer
 * 
 * Distribusi pertama dari kernel inos, QC Test untuk pengembangan
 * Notebook Axioo. Quality Control (QC) digunakan untuk menguji 
 * kualitas hardware dari produk komputer seperti pengetesan untuk
 * keyboard, touchpad, kamera, lcd, baterai, dll.
 *
 * Created by: Naufal Adriansyah
 * Thanks to : 
 *  - Dennis Ritchie (1941-2011) sebagai pembuat bahasa C dan Unix
 *  - Nick Blundel, pembuat buku "Write a Simple Operating System
 *	  from Scratch"
 *	- wiki.osdev.org 
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *	
 *	Copyright (C) Naufal Foundation 2019
 */


#include <tty.h>
static int x, y;
static const uint8_t rows, cols, depth;
static const uint8_t rows = 80, cols = 25, depth = 2;
#define TERMINAL_COLOR 0x07

void tty_clearline(uint8_t from, uint8_t to) {
	uint16_t i = rows * from * depth;
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

void tty_scroll(uint8_t lines) {
	char* vidmem = (char*) 0xb8000;
	uint16_t i = 0;
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

void tty_write(char* str) {
	while (*str) {
		tty_putch(*str++);	
	}
}

void tty_init(int _x, int _y, uint8_t _color) {
	tty_write("Initializing Teletype Writer... ");

	x = _x;
	y = _y;


	tty_write("[ OK ]\n");
	tty_clean();
}

void tty_cursor(uint8_t c_start, uint8_t c_end) {
	outb(0x3d4, 0x0a);
	outb(0x3d5, (inb(0x3d5) & 0xc0) | c_start);
	outb(0x3d4, 0x0b);
	outb(0x3d5, (inb(0x3d5) & 0xe0) | c_end);
}

void tty_printf(const char* fmt, ...) {
	char** arg = (char **) &fmt;
	int c;
	char buf[20];

	arg++;
	while ((c = *fmt++) != 0) {
		if (c != '%') {
			tty_putch(c);
		} else {
			char *p, *p2;
			int pad0 = 0, pad = 0;
			
			c = *fmt++;
			if (c == 0) {
				pad0 = 1;
				c = *fmt++;
			}

			switch (c) {
				case 'd':
				case 'u':
				// case 'x':
				// 	itoa(buf, c, *((int *) arg++));
				// 	p = buf;
				// 	goto string;
				// 	break;

				case 's':
					p = *arg++;
					if (! p)
						p = "(null)";

				string:
					for (p2 = p; *p2; p2++);
					for (; p2 < p + pad; p2++)
						tty_putch(pad0 ? '0' : ' ');
					while (*p)
						tty_putch(*p++);
					break;

				default:
					tty_putch(*((int *) arg++));
					break;
			}
		}
	}
}