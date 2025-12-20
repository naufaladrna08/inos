#include <tty.h>
#include <idt.h>

void write_string(const char *string) {
  while (*string) {
    tty_putch(*string++);
  }
}

int main(void) {
  tty_clean();
  // Setup IDT
  idt_init();
  write_string("Testing divide by zero...");
  
  int a = 10;
  int b = 0;
  int c = a / b;  // This will trigger interrupt 0
  
  (void)c;  // Prevent unused warning
  // Setup Paging
  // Run kernel main
  for (;;);
}