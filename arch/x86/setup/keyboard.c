#include "irq.h"
#include "pic.h"
#include <keyboard.h>
#include <tty.h>

static volatile uint8_t kbd_buffer[KBD_BUFF_SIZE];
static volatile uint8_t kbd_buff_head = 0;
static volatile uint8_t kbd_buff_tail = 0;

void kbd_push(uint8_t scancode) {
  kbd_buffer[kbd_buff_head] = scancode;
  kbd_buff_head = (kbd_buff_head + 1) % KBD_BUFF_SIZE;
}

void kbd_irq() {
  uint8_t scancode = inb(0x60);
  kbd_push(scancode);
}

void kbd_init() {
  irq_register(1, kbd_irq);
  pic_clear_mask(1);
}

bool kbd_has_data() {
  return kbd_buff_head != kbd_buff_tail;
}

uint8_t kbd_getc() {
  if (kbd_buff_head == kbd_buff_tail) {
    return 0; // No data
  }
  uint8_t scancode = kbd_buffer[kbd_buff_tail];
  kbd_buff_tail = (kbd_buff_tail + 1) % KBD_BUFF_SIZE;
  return scancode;
}
