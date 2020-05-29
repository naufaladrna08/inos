#ifndef ATA_H
#define ATA_H
#define ATA_MASTER          0x01
#define ATA_SLAVE           0x02
#define ATA_PRIMARY_PORT    0x1F0
#define ATA_SECONDARY_PORT  0x170 
#include <io.h>
#include <types.h>
#include <tty.h>

u16 data_port           = port_base;
u8  error_port          = port_base + 0x1;
u8  sector_count_port   = port_base + 0x2;
u8  lba_low_port        = port_base + 0x3;
u8  lba_middle_port     = port_base + 0x4;
u8  lba_high_port       = port_base + 0x5;
u8  device_port         = port_base + 0x6;
u8  command_port        = port_base + 0x7;
u8  control_port        = port_base + 0x206;

void ata_init(char master, u16 port_base);
void ata_identify();
void ata_write28(u32 sector_number, int count = 512);
void ata_read28(u32 sector_number, u8* data, u32 count);
void ata_flush();

#endif /* ata.h */