#include <io.h>

u8 inb(u16 _port) {
	__asm__("mov dx, %0" : : "g" (_port));
	__asm__("in al, dx");
}

u16 inw(u16 _port) {
	__asm__("mov dx, %0" : : "g" (_port));
	__asm__("inw ax, dx");
}

/**
 * Assembly instruction out in C
 * out port, data 
 */
void outb(u16 _port, u8 _data) {
	__asm__("mov dx, %0" : : "g" (_port));
	__asm__("mov al, %0" : : "g" (_data));
	__asm__("out dx, al");
}

void outw(u16 _port, u16 _data) {
	__asm__("mov dx, %0" : : "g" (_port));
	__asm__("mov ax, %0" : : "g" (_data));
	__asm__("outw dx, ax");
}

static __inline void insl(int port, void *addr, int cnt) {
	__asm __volatile("cld\n\trepne\n\tinsl"			:
			 "=D" (addr), "=c" (cnt)		:
			 "d" (port), "0" (addr), "1" (cnt)	:
			 "memory", "cc");
}