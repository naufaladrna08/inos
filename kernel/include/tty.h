#ifndef TTY_H
#define TTY_H
#include <types.h>
#include <io.h>

void tty_clearline(u8 from, u8 to);
void tty_clean();
void tty_refresh();
void tty_scroll(u8 lines);
void tty_checknl(); 
void tty_putch(char c);
void tty_write(char* str);
void tty_init(int _x, int _y, u8 _color);
void tty_cursor(u8 c_start, u8 c_end);
void tty_printf(const char* fmt, ...);

#endif /* tty.h */