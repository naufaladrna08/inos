BAR0 is the start of the I/O ports used by the primary channel.
BAR1 is the start of the I/O ports which control the primary channel.
BAR2 is the start of the I/O ports used by secondary channel.
BAR3 is the start of the I/O ports which control secondary channel.
BAR4 is the start of 8 I/O ports controls the primary channel's Bus Master IDE.
BAR4 + 8 is the Base of 8 I/O ports controls secondary channel's Bus Master IDE.

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