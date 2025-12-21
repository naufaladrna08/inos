#include <idt.h>

struct idt_entry idt[256];
struct idt_ptr idtptr;

void idt_init(void) {
  idtptr.limit = (sizeof(struct idt_entry) * 256) - 1;
  idtptr.base = (uint32_t) &idt;

  for (int i = 0; i < 256; i++) {
    idt_set_gate(i, 0, 0, 0);
  }

  idt_set_gate(0, (uint32_t) isr0, 0x08, 0x8E);
  idt_set_gate(1, (uint32_t) isr1, 0x08, 0x8E);
  idt_set_gate(13, (uint32_t) isr13, 0x08, 0x8E);
  idt_set_gate(14, (uint32_t) isr14, 0x08, 0x8E);
  
  idt_set_gate(32, (uint32_t) irq0, 0x08, 0x8E);
  idt_set_gate(33, (uint32_t) irq1, 0x08, 0x8E);

  idt_load((uint32_t) &idtptr);
}

void idt_set_gate(uint8_t n, uint32_t handler, uint16_t selector, uint8_t flags) {
  idt[n].offset_low = handler & 0xFFFF;
  idt[n].offset_high = (handler >> 16) & 0xFFFF;
  idt[n].selector = selector;
  idt[n].zero = 0;
  idt[n].type_attr = flags;
}