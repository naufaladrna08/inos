#include <tty.h>

void printf(const char* fmt, ...);

int main(void) {
  printf("Welcome to kernel");
  for (;;);
}

void printf(const char* fmt, ...) {
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
        //   itoa(buf, c, *((int *) arg++));
        //   p = buf;
        //   goto string;
        //   break;

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
