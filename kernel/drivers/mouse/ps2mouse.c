#include <drivers/mouse/ps2mouse.h>

#define CMDPORT 0x64
#define DTAPORT 0x60
#define IRQ_OFF { asm volatile ("cli"); }
#define IRQ_RES { asm volatile ("sti"); }

char buff[3];
unsigned int m_offset = 0;
u8 buttons;

void wait(char a_type) {
	int timeout = 100000;
	if (!a_type) {
		while (--timeout) {
			if ((inb(CMDPORT) & 0x01) == 1) {
				return;
			}
		}
		tty_write("mouse timeout");
		return;
	} else {
		while (--timeout) {
			if (!((inb(CMDPORT) & 0x02))) {
				return;
			}
		}
		tty_write("mouse timeout");
		return;
	}
}


#define KEY_DEVICE    0x60
#define KEY_PENDING   0x64

static char key_buffer[256];

void mouse_handler(struct regs *r) {
  unsigned char status = inb(CMDPORT);
  buff[m_offset] = inb(DTAPORT);

  m_offset = (m_offset + 1) % 3;
  static char mx = 80 / 2, my = 25 / 2;


  if (m_offset == 0) {
    static unsigned short* VideoMemory = (unsigned short*)0xb8000;
    VideoMemory[80*my+mx] = (VideoMemory[80*my+mx] & 0x0F00) << 4
                          | (VideoMemory[80*my+mx] & 0xF000) >> 4
                          | (VideoMemory[80*my+mx] & 0x00FF);

    mx += buff[1];
    if (mx >= 80) mx = 79;
    if (mx < 0) mx = 0;
    
    my -= buff[2];
    if (my >= 25) my = 24;
    if (my < 0) my = 0;

    VideoMemory[80*my+mx] = (VideoMemory[80*my+mx] & 0x0F00) << 4
                          | (VideoMemory[80*my+mx] & 0xF000) >> 4
                          | (VideoMemory[80*my+mx] & 0x00FF);
  } 

  irq_ack(12);
}

void mouse_install() {
  char status;
  m_offset = 0;
  buttons  = 0;
  tty_write("\nInitializing PS/2 Mouse Driver... ");

  unsigned short* VideoMemory = (unsigned short*)0xb8000;
  VideoMemory[80*12+40] = (VideoMemory[80*12+40] & 0x0F00) << 4
                        | (VideoMemory[80*12+40] & 0xF000) >> 4
                        | (VideoMemory[80*12+40] & 0x00FF);

  /*
   * beritahu kontroler untuk mengalamatkan mouse dan
   * tulis mouse command code ke data port kontroller. 
   */
  IRQ_OFF;
  outb(CMDPORT, 0xA8);
  outb(CMDPORT, 0x20);
    
  status = inb(DTAPORT) | 2;
  
  outb(CMDPORT, 0x60);
  outb(DTAPORT, status);
  
  // CMD=64
  outb(CMDPORT, 0xD4);
  outb(DTAPORT, 0xF4);

  inb(DTAPORT);

  IRQ_RES;
  irq_install_handler(12, mouse_handler);

  tty_write("[ OK ]\n");
}