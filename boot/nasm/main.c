void putc(char c) {
  asm volatile (
    "movb %[ch], %%al\n"
    "movb $0x0E, %%ah\n"
    "int $0x10\n"
    :
    : [ch] "r"(c)
    : "%eax"
  );
}

void printk(const char* str) {
  while (*str) 
    putc(*str++);
}

int main() {
  printk("Setup code loaded and running!\n");
  return 0;
}