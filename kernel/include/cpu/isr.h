#ifndef ISR_H
#define ISR_H
#include <system.h>

typedef void (*isr_t)(struct regs);
isr_t interrupt_handlers[256];

/* Implemented in isr.c */
extern void isr_install();
extern void _fault_handler(struct regs *r);

#endif