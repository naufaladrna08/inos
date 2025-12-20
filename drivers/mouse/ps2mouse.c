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

void mouse_handler(struct regs* r) {
  (void) r;

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

      /* Correctly handle signed movement values */
      s8 dx = (s8)packet[1];
      s8 dy = (s8)packet[2];

      /* VGA Text Mode Video Memory */
      static unsigned short* VideoMemory = (unsigned short*)0xb8000;
      
      /* Store previous cursor position */
      static u16 previous_char = 0;
      static u8 prev_x = 40, prev_y = 12;  // Initial position in the middle

      /* Restore original character at old position */
      VideoMemory[80 * prev_y + prev_x] = previous_char;

      /* Update mouse position */
      mouse_x += dx;
      mouse_y -= dy;  // Y axis is flipped in VGA text mode

      /* Ensure mouse stays within bounds */
      if (mouse_x >= 80) mouse_x = 79;
      if (mouse_x < 0) mouse_x = 0;
      if (mouse_y >= 25) mouse_y = 24;
      if (mouse_y < 0) mouse_y = 0;

      /* Store the character at the new position before modifying it */
      previous_char = VideoMemory[80 * mouse_y + mouse_x];

      /* Draw new cursor */
      VideoMemory[80 * mouse_y + mouse_x] ^= 0x7700;  // Invert colors to "draw"

      /* Update previous cursor position */
      prev_x = mouse_x;
      prev_y = mouse_y;

      /* Reset packet cycle */
      mouse_cycle = 0;
      break;
  }
  
  irq_ack(12);
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
  outb(DTAPORT, status);

  /* Set default settings */
  wait_controller_ready();
  outb(CMDPORT, 0xD4);
  wait_controller_ready();
  outb(DTAPORT, 0xF6);
  if (!wait_controller_response() || inb(DTAPORT) != 0xFA) return 0;

  /* Enable data reporting */
  wait_controller_ready();
  outb(CMDPORT, 0xD4);
  wait_controller_ready();
  outb(DTAPORT, 0xF4);
  if (!wait_controller_response() || inb(DTAPORT) != 0xFA) return 0;

  irq_install_handler(12, mouse_handler);
  return 1;
}