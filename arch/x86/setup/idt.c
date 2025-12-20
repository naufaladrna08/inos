#include <idt.h>
#include <tty.h>

struct idt_entry idt[256];
struct idt_ptr idtptr;

// Forward declarations for interrupt handlers
extern void idt_load(uint32_t);
extern void isr_div_zero(void);
extern void isr1(void);
extern void isr13(void);  // General Protection Fault
extern void isr14(void);  // Page Fault

void idt_init(void) {
  idtptr.limit = (sizeof(struct idt_entry) * 256) - 1;
  idtptr.base = (uint32_t) &idt;

  for (int i = 0; i < 256; i++) {
    idt_set_gate(i, 0, 0, 0);
  }

  idt_set_gate(0, (uint32_t) isr_div_zero, 0x08, 0x8E);
  idt_set_gate(1, (uint32_t) isr1, 0x08, 0x8E);
  idt_set_gate(13, (uint32_t) isr13, 0x08, 0x8E);
  idt_set_gate(14, (uint32_t) isr14, 0x08, 0x8E);

  idt_load((uint32_t) &idtptr);
}

void idt_set_gate(uint8_t n, uint32_t handler, uint16_t selector, uint8_t flags) {
  idt[n].offset_low = handler & 0xFFFF;
  idt[n].offset_high = (handler >> 16) & 0xFFFF;
  idt[n].selector = selector;
  idt[n].zero = 0;
  idt[n].type_attr = flags;
}

void isr_handler(uint32_t int_no, uint32_t err_code) {
  // Simple VGA text output
  volatile char *video = (volatile char*) 0xB8000;
  const char *msg = "INT:";
  
  // Print "INT:" at position
  for (int i = 0; i < 4; i++) {
    video[160 + i*2] = msg[i];      // Row 1, character
    video[160 + i*2 + 1] = 0x0C;    // Red text
  }
  
  // Print interrupt number (simple hex display)
  char hex[] = "0123456789ABCDEF";
  video[168] = hex[(int_no >> 4) & 0xF];
  video[169] = 0x0C;
  video[170] = hex[int_no & 0xF];
  video[171] = 0x0C;

  // Halt
  while(1) {
    __asm__ volatile("hlt");
  }
}

void isr_handler_0(uint32_t int_no, uint32_t err_code) {
  char* msg = "Cannot divide by 0\0";
  while (*msg) tty_putch(*msg++);

  while(1) {
    __asm__ volatile("hlt");
  }
}