#include <drivers/ata/ata.h>

/* Sleep Utilities  */
u32 jiffies = 0;
u16 hz      = 0;

void sleep(int sec) {
  u32 end = jiffies + sec * hz;
  while(jiffies < end);
}

/* Buffer untuk membaca identification space */
u8 ide_buffer[2048]        = {0};
static u8 ide_irq_invoked  = 0;
static u8 atapi_packets[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

u8 ide_read(u8 channel, u8 reg) {
  u8 result;
  
  if (reg > 0x07 && reg << 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nint);
  }

  if (reg < 0x08) {
    result = inb(channels[channel].base + reg - 0x00);
  } else if (reg < 0x0C) {
    result = inb(channels[channel].base  + reg - 0x06);
  } else if (reg < 0x0E) {
    result = inb(channels[channel].ctrl  + reg - 0x0A);
  } else if (reg < 0x16) {
    result = inb(channels[channel].bmide + reg - 0x0E);
  }
  
  if (reg > 0x07 && reg < 0x0C)
    ide_write(channel, ATA_REG_CONTROL, channels[channel].nint);
  
  return result;
}

u8 ide_write(u8 channel, u8 reg, u8 data) {
  if (reg > 0x07 && reg < 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nint);
  }
  
  if (reg < 0x08) {
    outb(channels[channel].base  + reg - 0x00, data);
  } else if (reg < 0x0C) {
    outb(channels[channel].base  + reg - 0x06, data);
  } else if (reg < 0x0E) {
    outb(channels[channel].ctrl  + reg - 0x0A, data);
  } else if (reg < 0x16) {
    outb(channels[channel].bmide + reg - 0x0E, data);
  }

  if (reg > 0x07 && reg < 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, channels[channel].nint);
  }
}

u8 ide_read_buffer(u8 channel, u8 reg, u32 buffer, u32 quads) {
  if (reg > 0x07 && reg < 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nint);
  }

  asm("pushw %es; movw %ds, %ax; movw %ax, %es");

  if (reg < 0x08) {
    insl(channels[channel].base  + reg - 0x00, buffer, quads);
  } else if (reg < 0x0C) {
    insl(channels[channel].base  + reg - 0x06, buffer, quads);
  } else if (reg < 0x0E) {
    insl(channels[channel].ctrl  + reg - 0x0A, buffer, quads);
  } else if (reg < 0x16) {
    insl(channels[channel].bmide + reg - 0x0E, buffer, quads);
  }

  asm("popw %es;");

  if (reg > 0x07 && reg < 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, channels[channel].nint);
  }
}

u8 ide_polling(u8 channel, u32 advanced_check) {
  for(int i = 0; i < 4; i++)
    ide_read(channel, ATA_REG_ALTSTATUS); 

  while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY);

  if (advanced_check) {
    unsigned char state = ide_read(channel, ATA_REG_STATUS);

    if (state & ATA_SR_ERR) {
      return 2;
    }

    if (state & ATA_SR_DF) {
      return 1; 
    }
    
    if ((state & ATA_SR_DRQ) == 0) {
      return 3; 
    }
  }

  return 0;
}

u8 ide_print_error(u32 drive, u8 err) {
  if (err == 0)
    return err;
 
  tty_write("IDE: ");
  if (err == 1) {
    tty_write("- Device Fault\n     "); err = 19;
  } else if (err == 2) {
    unsigned char st = ide_read(ide_devices[drive].channel, ATA_REG_ERROR);
    if (st & ATA_ER_AMNF) {
      tty_write("- No Address Mark Found\n");
      err = 7;
    }

    if (st & ATA_ER_TK0NF) { 
      tty_write("- No Media or Media Error\n");   
      err = 3;
    }
    
    if (st & ATA_ER_ABRT) {
      tty_write("- Command Aborted\n");      
      err = 20;
    }

    if (st & ATA_ER_MCR) {
      tty_write("- No Media or Media Error\n");   
      err = 3;
    }

    if (st & ATA_ER_IDNF) {
      tty_write("- ID mark not Found\n");      
      err = 21;
    }

    if (st & ATA_ER_MC) {
      tty_write("- No Media or Media Error\n");   
      err = 3;
    }

    if (st & ATA_ER_UNC) {
      tty_write("- Uncorrectable Data Error\n");
      err = 22;
    }

    if (st & ATA_ER_BBK) {
      tty_write("- Bad Sectors\n ");
      err = 13;
    }
  } else if (err == 3) {
    tty_write("- Reads Nothing\n"); 
    err = 23;
  } else if (err == 4)  {
    tty_write("- Write Protected\n");
    err = 8;
  }

  tty_printf("- [%s %s] %s\n",
  (const char *[]){"Primary", "Secondary"}[ide_devices[drive].channel],
  (const char *[]){"Master", "Slave"}[ide_devices[drive].drive],
  ide_devices[drive].model);
 
  return err;
}
void ide_initialize(u32 BAR0, u32 BAR1, u32 BAR2, u32 BAR3,u32 BAR4) {
  int j, k, count = 0;
 
  /* detect I/O Ports which interface IDE Controller: */
  channels[ATA_PRIMARY  ].base  = (BAR0 & 0xFFFFFFFC) + 0x1F0 * (!BAR0);
  channels[ATA_PRIMARY  ].ctrl  = (BAR1 & 0xFFFFFFFC) + 0x3F6 * (!BAR1);
  channels[ATA_SECONDARY].base  = (BAR2 & 0xFFFFFFFC) + 0x170 * (!BAR2);
  channels[ATA_SECONDARY].ctrl  = (BAR3 & 0xFFFFFFFC) + 0x376 * (!BAR3);
  channels[ATA_PRIMARY  ].bmide = (BAR4 & 0xFFFFFFFC) + 0; // Bus Master IDE
  channels[ATA_SECONDARY].bmide = (BAR4 & 0xFFFFFFFC) + 8; // Bus Master IDE

  /* disable irq */
  ide_write(ATA_PRIMARY  , ATA_REG_CONTROL, 2);
  ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);

  // 3- Detect ATA-ATAPI Devices:
  for (int i = 0; i < 2; i++)
    for (j = 0; j < 2; j++) {

        unsigned char err = 0, type = IDE_ATA, status;
        ide_devices[count].reserved = 0; // Assuming that no drive here.

        // (I) Select Drive:
        ide_write(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4)); // Select Drive.
        sleep(1); // Wait 1ms for drive select to work.

        // (II) Send ATA Identify Command:
        ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
        sleep(1); // This function should be implemented in your OS. which waits for 1 ms.
                  // it is based on System Timer Device Driver.

        // (III) Polling:
        if (ide_read(i, ATA_REG_STATUS) == 0) continue; // If Status = 0, No Device.

        while(1) {
          status = ide_read(i, ATA_REG_STATUS);
          if ((status & ATA_SR_ERR)) {err = 1; break;} // If Err, Device is not ATA.
          if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) break; // Everything is right.
        }

        // (IV) Probe for ATAPI Devices:

        if (err != 0) {
          unsigned char cl = ide_read(i, ATA_REG_LBA1);
          unsigned char ch = ide_read(i, ATA_REG_LBA2);

          if (cl == 0x14 && ch ==0xEB)
              type = IDE_ATAPI;
          else if (cl == 0x69 && ch == 0x96)
              type = IDE_ATAPI;
          else
              continue;

          ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
          sleep(1);
        }

        ide_read_buffer(i, ATA_REG_DATA, (unsigned int) ide_buffer, 128);

        ide_devices[count].reserved       = 1;
        ide_devices[count].type           = type;
        ide_devices[count].channel        = i;
        ide_devices[count].drive          = j;
        ide_devices[count].signature      = *((unsigned short *)(ide_buffer + ATA_IDENT_DEVICETYPE));
        ide_devices[count].capability     = *((unsigned short *)(ide_buffer + ATA_IDENT_CAPABILITIES));
        ide_devices[count].commands_set   = *((unsigned int *)(ide_buffer + ATA_IDENT_COMMANDSETS));

        if (ide_devices[count].commands_set & (1 << 26))
          ide_devices[count].size   = *((unsigned int *)(ide_buffer + ATA_IDENT_MAX_LBA_EXT));
        else
          ide_devices[count].size   = *((unsigned int *)(ide_buffer + ATA_IDENT_MAX_LBA));

        for(k = 0; k < 40; k += 2) {
          ide_devices[count].model[k] = ide_buffer[ATA_IDENT_MODEL + k + 1];
          ide_devices[count].model[k + 1] = ide_buffer[ATA_IDENT_MODEL + k];
        }
        ide_devices[count].model[40] = 0;

        count++;
    }

  for (int i = 0; i < 4; i++)
    if (ide_devices[i].reserved == 1) {
        tty_printf(" Found %s Drive %dGB - %s\n",
          (const char *[]){"ATA", "ATAPI"}[ide_devices[i].type],
          ide_devices[i].size / 1024 / 1024 / 2,
          ide_devices[i].model);
    }
}