#ifndef TTY_H
#define TTY_H
#include <io.h>

void tty_clearline(unsigned char from, unsigned char to);
void tty_clean();
void tty_refresh();
void tty_scroll(unsigned char lines);
void tty_checknl(); 
void tty_putch(char c);
void tty_cursor(unsigned char c_start, unsigned char c_end);

#endif /* tty.h */