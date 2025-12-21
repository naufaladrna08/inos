/*
 * This main file is a part of bootstrap code that runs in protected mode. After
 * setting up basic hardware, it transfers control to the kernel main function 
 * at 0x00100000.
 */

#include <keyboard.h>
#include <tty.h>
#include <idt.h>
#include <pic.h>

extern void load_kernel_to_1m();
extern void goto_kernel_main();

void write_string(const char *string) {
  while (*string) {
    tty_putch(*string++);
  }
}

int boot_main(void) {
  tty_clean();
  write_string("Booting...");
  // Setup IDT
  idt_init();
  pic_init();
  pic_set_mask(0);
  kbd_init();
  
  // Enable interrupts - CRITICAL!
  __asm__ volatile("sti");
  
  // Disable interrupts before jumping to kernel
  __asm__ volatile("cli");
  
  load_kernel_to_1m();
  goto_kernel_main();
  for (;;);
}
