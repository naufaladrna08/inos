#include <drivers/ata/ata.h>

struct ata_regs_t {
  u16 data_port;
  u16  error_port;
  u16  sector_count_port;
  u16  lba_low_port;
  u16  lba_middle_port;
  u16  lba_high_port;
  u16  device_port;
  u16  command_port;
  u16  control_port;
  u16  is_master;
}__attribute__((packed));

struct ata_regs_t regs[4];

void ata_init(int i, int master, u16 port_base) {
  /* I/O Registers */
  regs[i].data_port           = port_base;
  regs[i].error_port          = port_base + 0x1;
  regs[i].sector_count_port   = port_base + 0x2;
  regs[i].lba_low_port        = port_base + 0x3;
  regs[i].lba_middle_port     = port_base + 0x4;
  regs[i].lba_high_port       = port_base + 0x5;
  regs[i].device_port         = port_base + 0x6;
  regs[i].command_port        = port_base + 0x7;
  regs[i].control_port        = port_base + 0x206;

  regs[i].is_master = master - 1;
}

void ata_identify(int i) {
  outb(regs[i].device_port, regs[i].is_master ? 0xA0 : 0xB0);
  outb(regs[i].control_port, 0);

  tty_printf("I = 0x%x", regs[i].control_port);
  outb(regs[i].device_port, 0xA0);
  u8 status = inb(regs[i].command_port);
  if (status == 0xFF) {
    return;
  }
  
  outb(regs[i].device_port, regs[i].is_master ? 0xA0 : 0xB0);
  outb(regs[i].sector_count_port, 0);
  outb(regs[i].lba_low_port, 0);
  outb(regs[i].lba_middle_port, 0);
  outb(regs[i].lba_high_port, 0);
  outb(regs[i].command_port, 0xEC);
  
  status = inb(regs[i].command_port);
  if (status == 0x00) {
    return;
  }
  
  while(((status & 0x80) == 0x80)
      && ((status & 0x01) != 0x01))
      status = inb(regs[i].command_port);
      
  if (status & 0x01) {
    tty_printf("Error\n");
    return;
  }
  
  for (int i = 0; i < 256; i++) {
    u16 data = inw(regs[i].data_port);
    char *text = " \0";
    text[0] = (data >> 8) & 0xFF;
    text[1] = data & 0xFF;
    tty_printf("0x%x ", text);
  }
}

void ata_write28(u32 sector_number, int count) {

}

void ata_read28(u32 sector_number, u8* data, u32 count) {

}

void ata_flush() {

}
