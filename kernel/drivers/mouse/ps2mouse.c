#include <drivers/mouse/ps2mouse.h>

#define CMDPORT 0x64
#define DTAPORT 0x60
#define IRQ_OFF { asm volatile ("cli"); }
#define IRQ_RES { asm volatile ("sti"); }

static s8 mouse_x = 0;
static s8 mouse_y = 0;
static u8 mouse_buttons = 0;
static u8 mouse_cycle = 0;

#define KEY_DEVICE    0x60
#define KEY_PENDING   0x64

void wait_controller_ready() {
  /* wait until the buffer is empty */
  while (inb(CMDPORT) & 0x02);
}

int wait_controller_response() {
  int timeout = 1000;
  while (timeout-- && !(inb(CMDPORT) & 0x01));
  return timeout > 0;
}

void mouse_handler() {
  static char packet[3];
  u8 status = inb(CMDPORT);
  if (!(status & 0x20)) return;

  u8 data = inb(DTAPORT);
  switch (mouse_cycle) {
    case 0:
      packet[0] = data;
      if (!(data & 0x08)) return; // Bad first byte
      mouse_cycle++;
      break;
    case 1:
      packet[1] = data;
      mouse_cycle++;
      break;
    case 2:
      packet[2] = data;
      
      /* process packet */
      mouse_buttons = packet[0] & 0x07;
      mouse_x = packet[1];
      mouse_y = packet[2];

      /* reset cycle */
      mouse_cycle = 0;
      break;
  }
  
}

int mouse_install() {
  u8 status;

  /* Disable Interrupt */
  IRQ_OFF;

  /* Enable Auxilary Device */
  wait_controller_ready();
  outb(CMDPORT, 0xA8);

  /* Renable Interrupt */
  IRQ_RES;

  /* Get current configuration */
  wait_controller_ready();
  outb(CMDPORT, 0x20);
  if (!wait_controller_response()) return 0;
  status = inb(DTAPORT) | 0x02;

  /* Set configuration */
  wait_controller_ready();
  outb(CMDPORT, 0x60);
  wait_controller_ready();
  outb(CMDPORT, status);

  /* Set default settings */
  wait_controller_ready();
  outb(CMDPORT, 0xD4);
  wait_controller_ready();
  outb(CMDPORT, 0xF6);
  if (!wait_controller_response() || inb(DTAPORT) != 0xFA) return 0;

  /* Enable data reporting */
  wait_controller_ready();
  outb(CMDPORT, 0xD4);
  wait_controller_ready();
  outb(CMDPORT, 0xF4);
  if (!wait_controller_response() || inb(DTAPORT) != 0xFA) return 0;

  irq_install_handler(12, mouse_handler);
  return 1;
}