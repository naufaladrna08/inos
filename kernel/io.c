#include <io.h>

u8 inb(u16 _port) {
	__asm__("mov dx, %0" : : "g" (_port));
	__asm__("in al, dx");
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