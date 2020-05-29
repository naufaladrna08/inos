#include <drivers/ata/ata.h>

void ata_init(char master, u16 port_base) {
  /* I/O Registers */
  data_port           = port_base;
  error_port          = port_base + 0x1;
  sector_count_port   = port_base + 0x2;
  lba_low_port        = port_base + 0x3;
  lba_middle_port     = port_base + 0x4;
  lba_high_port       = port_base + 0x5;
  device_port         = port_base + 0x6;
  command_port        = port_base + 0x7;
  control_port        = port_base + 0x206;

  is_master = master;
}

void ata_identify() {
  outb(device_port, is_master ? 0xA0 : 0xB0);
  outb(control_port, 0);

  outb(device_port, 0xA0);
  u8 status = inb(command_port);
  if(status == 0xFF)
    return;
  
  outb(device_port, is_master ? 0xA0 : 0xB0);
  outb(sector_count_port, 0);
  outb(lba_low_port, 0);
  outb(lba_middle_port, 0);
  outb(lba_high_port, 0);
  outb(command_port, 0xEC);
  
  status = inb(command_port);
  if(status == 0x00)
    return;
  
  while(((status & 0x80) == 0x80)
      && ((status & 0x01) != 0x01))
      status = inb(command_port);
      
  if(status & 0x01) {
    tty_printf("Error\n");
    return;
  }
  
  for(int i = 0; i < 256; i++) {
    u16 data = inw(data_port);
    char *text = "  \0";
    text[0] = (data >> 8) & 0xFF;
    text[1] = data & 0xFF;
    tty_printf(text);
  }
}

void ata_write28(u32 sector_number, int count = 512) {

}

void ata_read28(u32 sector_number, u8* data, u32 count) {

}

void ata_flush() {

}
