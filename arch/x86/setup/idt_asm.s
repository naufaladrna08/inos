.intel_syntax noprefix
.global idt_load

.global isr0, isr1, isr13, isr14
.global irq0, irq1

# -----------------------------
# Load IDT
# void idt_load(uint32_t *ptr)
# -----------------------------
idt_load:
  mov eax, [esp + 4]
  lidt [eax]
  ret

# -----------------------------
# ISR macros
# -----------------------------
.macro ISR_NOERR num
.global isr\num
isr\num:
  cli
  push 0          # dummy error code
  push \num       # int_no
  jmp isr_common_stub
.endm

.macro ISR_ERR num
.global isr\num
isr\num:
  cli
  push \num       # int_no (CPU already pushed err_code)
  jmp isr_common_stub
.endm

ISR_NOERR 0
ISR_NOERR 1
ISR_ERR   13
ISR_ERR   14

# -----------------------------
# IRQ macros (PUSH INTERRUPT NUMBER!)
# -----------------------------
.macro IRQ num, intno
.global irq\num
irq\num:
  cli
  push 0
  push \intno     # 32 / 33 !!!
  jmp irq_common_stub
.endm

IRQ 0, 32
IRQ 1, 33

# -----------------------------
# Common ISR stub
# -----------------------------
isr_common_stub:
  pusha
  push ds
  push es
  push fs
  push gs

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov eax, [esp + 48]   # int_no
  mov ebx, [esp + 52]   # err_code

  push ebx
  push eax
  call isr_handler
  add esp, 8

  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8
  iret

# -----------------------------
# Common IRQ stub
# -----------------------------
irq_common_stub:
  pusha
  push ds
  push es
  push fs
  push gs

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov eax, [esp + 48]   # int_no (32+)

  push eax
  call irq_handler
  add esp, 4

  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8
  iret
