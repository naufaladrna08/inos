.code32
.intel_syntax noprefix
.global _start

_start:
  # Now set up segment registers
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  # Set up stack pointer
  mov esp, 0x9F000

  rep stosb
  cli

  # Call the C kernel entry point
  # extern void kernel_init(unsigned long magic, unsigned long addr)
  .extern boot_main
  call boot_main

  # If kernel returns, halt
  jmp .halt_protected

.halt_protected:
  cli
  hlt
  jmp .halt_protected

.global load_kernel_to_1m
load_kernel_to_1m:
  # Debug: Check what's at 0x10000 before copy
  mov al, byte ptr [0x010000]
  mov byte ptr [0xb8000 + 164], al
  mov byte ptr [0xb8000 + 165], 0x0A
  
  mov al, byte ptr [0x010001]
  mov byte ptr [0xb8000 + 166], al
  mov byte ptr [0xb8000 + 167], 0x0A
  
  # Copy the kernel from 0x010000 to memory address 0x100000 (1MB)
  mov esi, 0x010000
  mov edi, 0x100000
  mov ecx, 0x8000   # Number of bytes to copy (32KB - enough for ~10KB kernel)
  cld
  rep movsb
  
  # Debug: Check what's at 0x100000 after copy
  mov al, byte ptr [0x100000]
  mov byte ptr [0xb8000 + 168], al
  mov byte ptr [0xb8000 + 169], 0x0B
  
  ret

.global goto_kernel_main
goto_kernel_main:
  # Debug: write 'J' to screen before jump
  mov byte ptr [0xb8000 + 160], 'J'
  mov byte ptr [0xb8000 + 161], 0x0C
  
  # Jump to the kernel's entry point at 1MB
  # Make sure segments are set up
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  
  # Set up stack for kernel
  mov esp, 0x9E000
  
  # Debug: Check first bytes at 0x100000
  mov al, byte ptr [0x100000]
  mov byte ptr [0xb8000 + 162], al
  mov byte ptr [0xb8000 + 163], 0x0E
  
  # Near jump to kernel
  mov eax, 0x00100000
  jmp eax
