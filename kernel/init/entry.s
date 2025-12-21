.code32
.intel_syntax noprefix
.global _start
// .extern main

_start:
  # Test print 'A' character with 0xb8000
  mov byte ptr [0xb8000], 'A'
  mov byte ptr [0xb8000 + 1], 0x07

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov ss, ax

  mov esp, 0x9E000

  call main

.halt:
  hlt
  jmp .halt
