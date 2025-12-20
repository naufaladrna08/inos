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

  # Clear BSS section (if needed)
  // mov edi, offset __bss_start
  // mov ecx, offset __bss_end
  // sub ecx, edi
  // xor eax, eax
  // rep stosb

  # Call the C kernel entry point
  # extern void kernel_init(unsigned long magic, unsigned long addr)
  .extern main
  push 0              # addr parameter (no multiboot info)
  push 0              # magic parameter (we're not multiboot)
  call main

  # If kernel returns, halt
  cli
  jmp .halt_protected

.halt_protected:
  cli
  hlt
  jmp .halt_protected