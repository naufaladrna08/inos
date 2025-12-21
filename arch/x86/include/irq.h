#ifndef IRQ_H
#define IRQ_H
#include <pic.h>

/* Keyboard */
extern void irq0(void);
extern void irq1(void);

typedef void (*irq_handler_t)(void);
void irq_register(int irq, irq_handler_t handler);
void irq_handler(int int_no);

#endif