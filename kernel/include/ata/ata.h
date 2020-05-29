#ifndef ATA_H
#define ATA_H
#define ATA_MASTER          0x01
#define ATA_SLAVE           0x02
#define ATA_PRIMARY_PORT    0x1F0
#define ATA_SECONDARY_PORT  0x170 
#include <io.h>
#include <types.h>

void ata_init(char master, u16 port_base);
void ata_identify();
void ata_write28(u32 sector_number, int count = 512);
void ata_read28(u32 sector_number, u8* data, u32 count);
void ata_flush();

#endif /* ata.h */