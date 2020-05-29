/**
 * idt.h
 * 
 * IDT is used in order to show the processor what Interrupt
 * Service Routines (ISRs) to call, to handle either an exception
 * or an 'int' opcode (in assembly). IDT entries a.k.a Interrupt
 * Requests (IRQ) whenever device hs completed a request and need
 * to be serviced.
 * 
 * (C) Naufal Foundation 2019
 */

#ifndef IDT_H
#define IDT_H
#include <types.h>

extern void idt_set_gate(int n, u32 handler);
extern void idt_install();

#endif