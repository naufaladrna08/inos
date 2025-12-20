# idt_asm.s
.intel_syntax noprefix
.global idt_load
.global isr0, isr1, isr13, isr14

# Load IDT
# void idt_load(uint32_t idt_ptr_address)
idt_load:
  mov eax, [esp + 4]    # Get pointer to IDT pointer struct
  lidt [eax]            # Load IDT
  ret

# Macro for ISRs without error codes
.macro ISR_NOERRCODE num
  .global isr\num
  isr\num:
      cli
      push 0                # Dummy error code
      push \num             # Interrupt number
      jmp isr_common_stub
.endm

# Macro for ISRs with error codes (CPU pushes these automatically)
.macro ISR_ERRCODE num
  .global isr\num
  isr\num:
      cli
      push \num             # Interrupt number (error code already pushed)
      jmp isr_common_stub
.endm

# Define interrupt service routines
ISR_NOERRCODE 0     # Divide by zero
ISR_NOERRCODE 1     # Debug
ISR_ERRCODE 13      # General Protection Fault (has error code)
ISR_ERRCODE 14      # Page Fault (has error code)

# Common ISR stub
isr_common_stub:
  # Save all registers
  pusha
  
  push ds
  push es
  push fs
  push gs
  
  # Load kernel data segment
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  
  # Push error code and interrupt number
  push esp              # Push pointer to stack (for struct access)
  
  # Call C handler
  call isr_handler
  
  # Restore registers
  add esp, 4            # Clean up pushed ESP
  pop gs
  pop fs
  pop es
  pop ds
  popa
  
  # Clean up error code and interrupt number
  add esp, 8
  
  iret                  # Return from interrupt

.global isr_div_zero
isr_div_zero:
  # Save all registers
  pusha
  
  push ds
  push es
  push fs
  push gs
  
  # Load kernel data segment
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  
  # Push error code and interrupt number
  push esp              # Push pointer to stack (for struct access)
  
  # Call C handler
  call isr_handler_0
  
  # Restore registers
  add esp, 4            # Clean up pushed ESP
  pop gs
  pop fs
  pop es
  pop ds
  popa
  
  # Clean up error code and interrupt number
  add esp, 8
  
  iret                  # Return from interrupt
