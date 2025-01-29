#include <cpu/isr.h>
#include <cpu/idt.h>
#include <cpu/irq.h>
#include <cpu/gdt.h>
#include <drivers/mouse/ps2mouse.h>
#include <drivers/kbd/ps2keyboard.h>
#include <drivers/ata/ata.h>
#include <multiboot.h>
#include <tty.h>
#include <string.h>
#include <kernel.h>
// #include <cpu/reboot.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))
// #define CHECK_VALUE(cond, strifeq) if (cond) printf("%s\n", strifeq);

void printf(const char* fmt, ...);

/* print the multiboot structure */
int kernel_init(unsigned long magic, unsigned long addr) {
  
  tty_init(0, 0, 0x0F);

  /* Initialize the core modules */
  gdt_install();
  isr_install();
  irq_install();
  idt_install();

  multiboot_info_t *mbi;
  printf("Multiboot info: \n");
  printf("The multiboot magic is: %x, ", magic);
  if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
    printf("which is correct!");
  } else {
    printf("which is incorrect!");
  }

  /* Set MBI to the address of the Multiboot information structure. */
  mbi = (multiboot_info_t *) addr;

  /* Print out the flags. */
  printf("\nFlags = 0x%x\n", (unsigned) mbi->flags);

  /* Are mem_* valid? */
  if (CHECK_FLAG(mbi->flags, 0))
    printf("Lower Memory = %uKB, Upper Memory = %uKB\n", (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

  /* Is boot_device valid? */
  if (CHECK_FLAG(mbi->flags, 1))
    printf("Boot Device = 0x%x\n", (unsigned) mbi->boot_device);
  
  /* Is the command line passed? */
  if (CHECK_FLAG (mbi->flags, 2))
    printf("Command Line = %s\n", (char *) mbi->cmdline);


  if (CHECK_FLAG(mbi->flags, 12)) {
    printf("This kernel is not run in VBE mode.");
  } else {
    printf("This kernel is run in VBE mode.");
  }

  /* input devices */
  printf("\nLoading drivers...");
  if (!mouse_install) {
    printf("Mouse initialization failed!\n");
  }
  keyboard_install();
}


void cmd_input(char *cmd) {
  if (strcmp(cmd, "end") == 0) {
    reboot();
  } else if (strcmp(cmd, "reboot") == 0) {
    reboot();
  } else if (strcmp(cmd, "clear") == 0) {
    tty_clean();
  } else {
    printf("\nUnknown command: %s\n", cmd);
  }
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
        case 'x':
          itoa(buf, c, *((int *) arg++));
          p = buf;
          goto string;
          break;

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
