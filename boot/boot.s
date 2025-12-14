.code16
.global _start

_start:
  cli
  mov %dl, boot_drive
  xor %ax, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %ss
  mov $0x7c00, %sp
  sti
  cld

  # Printing hello message with BIOS print function
  mov $hello_message, %si
  call print_loop

  # Read stage 2
  call read_sector

  # Jump to stage2
  ljmp $0x0000, $0x8000

print_loop:
  # Load string to al
  lodsb 
  test %al, %al
  jz .ret

  mov $0x0e, %ah
  mov $0, %bh
  int $0x10

  jmp print_loop

.ret:
  ret

# https://wiki.osdev.org/Disk_access_using_the_BIOS_(INT_13h)
read_sector:
  mov $0x02, %ah
  mov $1, %al
  mov $0, %ch
  mov $2, %cl
  mov $0, %dh
  mov boot_drive, %dl   # boot drive
  xor %ax, %ax
  mov %ax, %es
  mov $0x8000, %bx
  int $0x13
  jc disk_error
  ret

disk_error:
  mov $disk_error_message, %si
  call print_loop
  hlt

hello_message:
  .asciz "Testing..."
disk_error_message:
  .asciz "Failed to read disk. halting..."
boot_drive:
    .byte 0

/* Pad to 510 bytes and add signature */
.fill 510 - (. - _start), 1, 0
.word 0xaa55
