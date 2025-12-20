#include <cpu/isr.h>
#include <cpu/idt.h>
#include <cpu/irq.h>
#include <cpu/gdt.h>
#include <drivers/mouse/ps2mouse.h>
#include <drivers/kbd/ps2keyboard.h>
#include <drivers/disk/floppy.h>
#include <multiboot.h>
#include <tty.h>
#include <string.h>
#include <kernel.h>
// #include <cpu/reboot.h>
#include <memory.h>

void floppy_delay() {
  for (volatile int i = 0; i < 100000; i++); // Adjust the loop count as needed
}

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

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
  printf("\nLoading drivers...\n");
  if (mouse_install() == -1) printf("\nFailed to initialize mouse\n");
  keyboard_install();

  
  /* Testing Floppy driver */
  printf("\nTesting floppy driver...\n");
  floppy_init();
  printf("Floppy driver initialized\n");

  /* Give the floppy drive some time to initialize */
  floppy_delay();

  /* Testing write data to sector and read it */
  printf("\nTesting write data to sector and read it...\n");
  u8 buffer[512];
  memset(buffer, 0, 512);
  buffer[0] = 'A';
  buffer[1] = 'B';
  buffer[2] = 'C';

  /* Write data to sector 1 */
  if (floppy_write_sector(0, 0, 1, buffer)) {
    printf("Data written to sector 1\n");
  } else {
    printf("Failed to write data to sector 1\n");
    return 1; // Exit if write fails
  }

  /* Give the floppy drive some time to complete the write operation */
  floppy_delay();

  /* Clear the buffer before reading */
  memset(buffer, 0, 512);

  /* Read data from sector 1 */
  if (floppy_read_sector(0, 0, 1, buffer)) {
    printf("Data read from sector 1:\n");

    /* Print the first few bytes of the buffer */
    for (int i = 0; i < 10; i++) {
      printf("%02X ", buffer[i]); // Print each byte in hexadecimal
    }
    printf("\n");

    /* Verify the data */
    if (buffer[0] == 'A' && buffer[1] == 'B' && buffer[2] == 'C') {
      printf("Data verification successful!\n");
    } else {
      printf("Data verification failed!\n");
    }
  } else {
    printf("Failed to read data from sector 1\n");
    return 1; // Exit if read fails
  }
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
