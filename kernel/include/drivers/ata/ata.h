#ifndef ATA_H
#define ATA_H
#include <io.h>
#include <types.h>
#include <tty.h>

/* Channels. */
#define ATA_PRIMARY      0x00
#define ATA_SECONDARY    0x01
 
/* Directions */
#define ATA_READ      0x00
#define ATA_WRITE     0x01

#define IDE_ATA        0x00
#define IDE_ATAPI      0x01
 
#define ATA_MASTER          0
#define ATA_SLAVE           1

#define ATA_PRIMARY_PORT    0x1F0
#define ATA_SECONDARY_PORT  0x170 

/* Status Ports */
#define ATA_SR_BSY     0x80    // Busy
#define ATA_SR_DRDY    0x40    // Drive ready
#define ATA_SR_DF      0x20    // Drive write fault
#define ATA_SR_DSC     0x10    // Drive seek complete
#define ATA_SR_DRQ     0x08    // Data request ready
#define ATA_SR_CORR    0x04    // Corrected data
#define ATA_SR_IDX     0x02    // Index
#define ATA_SR_ERR     0x01    // Error

/* Error Ports */
#define ATA_ER_BBK      0x80    // Bad block
#define ATA_ER_UNC      0x40    // Uncorrectable data
#define ATA_ER_MC       0x20    // Media changed
#define ATA_ER_IDNF     0x10    // ID mark not found
#define ATA_ER_MCR      0x08    // Media change request
#define ATA_ER_ABRT     0x04    // Command aborted
#define ATA_ER_TK0NF    0x02    // Track 0 not found
#define ATA_ER_AMNF     0x01    // No address mark

/* Commands */
#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC
/* Command untuk ATAPI */
#define ATAPI_CMD_READ            0xA8
#define ATAPI_CMD_EJECT           0x1B

/*
 * ATA_CMD_IDENTIFY_PACKET dan ATA CMD IDENTIFY mengembalikan
 * buffer 512 bytes yang dinamakan `identification space`, 
 * berikut adalah yang digunakan untuk membaca informasi di 
 * identification space.
 */
#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200

/*
 * Task File adalah jangkauan dari 8 port yang mana adalah
 * port dari BAR0 (chanel utama) dan/atau BAR2 (chanel kedua).
 */
#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

/*
 * Port ALTSTATUS / CONTROL mengembalikan status alternatif 
 * saat membaca dan mengontrol saluran saat ditulis.
 * Untuk channel utama, ALTSTATUS/CONTROL = BAR1 + 2
 * Untuk channel kedua, ALTSTATUS/CONTROL = BAR3 + 2
 */

struct ide_channel_regs {
  u16 base;  /* i/o base */
  u16 ctrl;  /* control base */
  u16 bmide; /* BUS master IDE */
  u8  nint;  /* no interrupt */
} channels[2];

struct ide_device {
  u8  reserved;
  u8  channel;
  u8  drive;
  u16 type;
  u16 signature;
  u16 capability;
  u32 commands_set;
  u32 size;
  u8  model[4];
} ide_devices[4];

/* ATA IDE FUNCTIONS */
u8 ide_read(u8 channel, u8 reg);
u8 ide_write(u8 channel, u8 reg, u8 data);
u8 ide_read_buffer(u8 channel, u8 reg, u32 buffer, u32 quads);
u8 ide_polling(u8 channel, u32 advanced_check);
u8 ide_print_error(u32 drive, u8 err);
void ide_initialize(u32, u32 BAR1, u32 BAR2, u32 BAR3,u32 BAR4);

#endif /* ata.h */