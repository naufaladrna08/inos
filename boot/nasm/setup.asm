[BITS 16]
[EXTERN main]

; Call c main function after setting up stack and segments
start:
  cli
  xor ax, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  mov sp, 0x9fff  ; Setup stack
  sti

  mov si, message_from_setup
  call bios_print

  call main

hang:
  jmp hang

bios_print:
  lodsb
  or al, al
  jz ret

  mov ah, 0x0E
  int 0x10

  jmp bios_print
ret:
  ret

message_from_setup db "Hello from setup code!", 0