#ifndef ISR_H
#define ISR_H
#include <types.h>

// Forward declarations for interrupt handlers
extern void idt_load(uint32_t);
extern void isr0(void);
extern void isr1(void);
extern void isr13(void);  // General Protection Fault
extern void isr14(void);  // Page Fault

void isr_handler(uint32_t int_no, uint32_t err_code);

#endif