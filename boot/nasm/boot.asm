[BITS 16]
[org 0x7c00]

start:
  cli
  mov [boot_drive], dl
  xor ax, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  mov sp, 0x7c00
  cld

  mov si, message
  call bios_print

  ; Now we have print the message, we can read the setup sector
  ; to load the rest of the bootloader
  call read_setup_sector

  ; Far jump to the setup code at 0x8000
  jmp 0x0000:0x8000

bios_print:
  lodsb
  or al, al
  jz ret

  mov ah, 0x0E
  int 0x10

  jmp bios_print

ret:
  ret

read_setup_sector:
  mov ah, 0x02        ; BIOS read sector function
  mov al, 1           ; Number of sectors to read
  mov ch, 0           ; Cylinder 0
  mov cl, 2           ; Sector 2 (setup sector)
  mov dh, 0           ; Head 0
  mov bx, 0x8000      ; Buffer to load the sector
  mov dl, [boot_drive]; Boot drive
  int 0x13
  jc disk_error_handler
  ret

disk_error_handler:
  mov si, disk_error
  call bios_print
  hlt

message db "Hello, World from bootloader!", 0
disk_error db "Disk Error!", 0
boot_drive db 0x00

times 510-($-$$) db 0
dw 0xAA55