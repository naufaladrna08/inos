# Ata Device (Perangkat ATA)

- BAR0 adalah awal dari port I / O yang digunakan oleh saluran utama.
- BAR1 adalah awal dari port I / O yang mengontrol saluran utama.
- BAR2 adalah awal dari port I / O yang digunakan oleh saluran sekunder.
- BAR3 adalah awal dari port I / O yang mengontrol saluran sekunder.
- BAR4 adalah awal dari 8 port I / O yang mengontrol Bus Master IDE saluran utama.
- BAR4 + 8 adalah Basis dari 8 port I / O yang mengontrol Bus Master IDE saluran sekunder. 

```c
struct ide_device {
  unsigned char  Reserved;    // 0 (Empty) or 1 (This Drive really exists).
  unsigned char  Channel;     // 0 (Primary Channel) or 1 (Secondary Channel).
  unsigned char  Drive;       // 0 (Master Drive) or 1 (Slave Drive).
  unsigned short Type;        // 0: ATA, 1:ATAPI.
  unsigned short Signature;   // Drive Signature
  unsigned short Capabilities;// Features.
  unsigned int   CommandSets; // Command Sets Supported.
  unsigned int   Size;        // Size in Sectors.
  unsigned char  Model[41];   // Model in string.
} ide_devices[4];
```
