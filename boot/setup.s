.code16
.global _start  # Changed from 'setup' to '_start'

_start:
  # Set up segment registers after being loaded to 0x8000
  xor %ax, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %ss
  mov $0x7c00, %sp  # Set stack pointer (can be any valid location)
  cld

  mov $msg, %si
  call print_loop
  
hang:
  jmp hang

print_loop:
  lodsb 
  test %al, %al
  jz .ret
  mov $0x0e, %ah
  mov $0, %bh
  int $0x10
  jmp print_loop
.ret:
  ret

msg:
  .asciz "Setup"
  