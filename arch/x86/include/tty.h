#ifndef TTY_H
#define TTY_H
#include <string.h>

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

static inline void outb(uint16_t port, uint8_t value) {
  asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Read a byte from an I/O port
static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

void tty_clearline(unsigned char from, unsigned char to);
void tty_clean();
void tty_refresh();
void tty_scroll(unsigned char lines);
void tty_checknl(); 
void tty_putch(char c);
void tty_cursor(unsigned char c_start, unsigned char c_end);

#endif /* tty.h */