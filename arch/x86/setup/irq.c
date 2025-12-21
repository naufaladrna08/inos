#include <irq.h>
#include <pic.h>
#include <tty.h>

static irq_handler_t irq_handlers[16];

void irq_register(int irq, irq_handler_t handler) {
  irq_handlers[irq] = handler;
}

void irq_handler(int int_no) {
  uint8_t irq = int_no - 32;

  if (irq < 16 && irq_handlers[irq])
    irq_handlers[irq]();

  pic_send_eoi(irq);
}