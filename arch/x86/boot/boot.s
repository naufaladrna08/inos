/*
 * Real-mode bootloader for x86 architecture  
 */

.code16
.intel_syntax noprefix
.global _start

_start:
  cli
  cld

  # Copy boot sector to new (safer location)
  mov ax, 0x07c0
  mov ds, ax
  mov ax, 0x9000
  mov es, ax
  sub si, si
  sub di, di
  mov cx, 512
  rep movsb

  ljmp 0x9000, boot_start

boot_start:
  # Set up segment registers to known state
  cli                     # Disable interrupts while setting up stack
  mov ax, 0x9000
  mov ds, ax              # Set data segment to 0
  mov es, ax              # Extra segment to 0
  mov ss, ax              # Stack segment to 0
  mov sp, 0xFFFE          # Stack pointer below our code
  sti                     # Re-enable interrupts

  mov al, 'A'
  mov ah, 0x0E
  int 0x10

  # Read setup sector from disk into memory before entering PM
  // mov [boot_drive], dl
  // mov ah, 0x02         # BIOS read sector function
  // mov al, 4            # Number of sectors to read (4 sectors = 2048 bytes)
  // mov ch, 0            # Cylinder 0
  // mov cl, 2            # Sector 2 (setup sector)
  // mov dh, 0            # Head 0
  // mov bx, 0x8000       # Buffer to load the sector
  // mov dl, [boot_drive] # Boot drive
  // int 0x13
  // jc disk_error

  // jmp 0x0000:0x8000
hang:
  jmp hang  

boot_drive:
  .byte 0

/* Pad to 510 bytes and add signature */
.fill 510 - (. - _start), 1, 0
.word 0xAA55
