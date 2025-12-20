/*
 * Real-mode bootloader for x86 architecture  
 */

.code16
.intel_syntax noprefix
.global _start

# 0x7C00 = Start
# 0x8000 = Init

_start:
  cli
  cld

  # Copy boot sector to new (safer location)
  mov ax, 0x07c0
  mov ds, ax
  mov ax, 0x8000
  mov es, ax
  sub si, si
  sub di, di
  mov cx, 512
  rep movsb

  ljmp 0x8000, boot_start

boot_start:
  # Set up segment registers to known state
  cli                     # Disable interrupts while setting up stack
  mov ax, 0x8000
  mov ds, ax              # Set data segment to 0
  mov es, ax              # Extra segment to 0
  mov ss, ax              # Stack segment to 0
  mov sp, 0xFFFE          # Stack pointer below our code
  sti                     # Re-enable interrupts

  # Read setup sector from disk into memory before entering PM
  mov [boot_drive], dl
  mov ax, 0x0000
  mov es, ax
  mov ah, 0x02         # BIOS read sector function
  mov al, 8            # Number of sectors to read (8 sectors = 4096 bytes)
  mov ch, 0            # Cylinder 0
  mov cl, 2            # Sector 2 (setup sector)
  mov dh, 0            # Head 0
  mov bx, 0x9000       # Buffer to load the sector temporary
  mov dl, [boot_drive] # Boot drive
  int 0x13
  jc disk_error

  # Enable A20 so we can access memory >1MB (in 8086)
  # We'll use fast method because I don't care backward-compatible. Most newer 
  # hardware model and most emulator supports it. 
  in al, 0x92
  test al, 2
  jnz after
  or al, 2
  and al, 0xFE
  out 0x92, al
  after:

after:
  lea si, a20_pass_msg
  call print_loop

  # Clear interrupt before loading GDT
  cli 

  # Load GDT
  lgdt [gdt_descriptor]

  mov eax, cr0
  or eax, 1
  mov cr0, eax 

  # Jump to protected-mode
  ljmp 0x08, 0x9000

hang:
  jmp hang  

boot_drive:
  .byte 0

disk_error:
  lea si, disk_error_msg
  call print_loop

print_loop:
  lodsb
  or al, al 
  je print_done
  mov ah, 0x0E
  int 0x10
  jmp print_loop

print_done:
  ret

disk_error_msg:
  .asciz "Disk read error"
a20_pass_msg:
  .asciz "A20 fast method passed, hopefully nothing wrong"

.align 8
gdt_start:
  gdt_null:
    .quad 0
  gdt_code:
    .long 0x0000FFFF    # Limit=0xFFFF, Base(0-15)=0x0000
    .long 0x00CF9A00      # Base(24-31)=0x00, Flags+Limit=0xCF, Access=0x9A, Base(16-23)=0x00

  gdt_data:
    .long 0x0000FFFF    # Limit=0xFFFF, Base(0-15)=0x0000
    .long 0x00CF9200    # Base(24-31)=0x00, Flags+Limit=0xCF, Access=0x92, Base(16-23)=0x00
gdt_end:

gdt_descriptor:
  .word gdt_end - gdt_start - 1
  .long gdt_start + 0x80000 
  
/* Pad to 510 bytes and add signature */
.fill 510 - (. - _start), 1, 0
.word 0xAA55
