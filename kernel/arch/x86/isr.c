/*
 * Interrupt Service Routines
 * 
 * Created by: Naufal Adriansyah
 * Thanks to : 
 *  - Dennis Ritchie (1941-2011) sebagai pembuat bahasa C dan Unix
 *  - Nick Blundel, pembuat buku "Write a Simple Operating System
 *	  from Scratch"
 *	- wiki.osdev.org 
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *	
 *	Copyright (C) Naufal Foundation 2019
 */


#include <cpu/isr.h>
#include <cpu/idt.h>
#include <tty.h>
#define SYSCALL_VECTOR 0x7F

static irq_handler_t isr_routines[32];
isr_t interrupt_handlers[256];

/* x86 Standard ISRs. Implemented in boot.S (Assembly) */
extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();

void isr_install() {
	tty_write("Setting up Interrupt Service Routines... ");
	idt_set_gate(0, (u32)_isr0);
	idt_set_gate(1, (u32)_isr1);
	idt_set_gate(2, (u32)_isr2);
	idt_set_gate(3, (u32)_isr3);
	idt_set_gate(4, (u32)_isr4);
	idt_set_gate(5, (u32)_isr5);
	idt_set_gate(6, (u32)_isr6);
	idt_set_gate(7, (u32)_isr7);
	idt_set_gate(8, (u32)_isr8);
	idt_set_gate(9, (u32)_isr9);
	idt_set_gate(10, (u32)_isr10);
	idt_set_gate(11, (u32)_isr11);
	idt_set_gate(12, (u32)_isr12);
	idt_set_gate(13, (u32)_isr13);
	idt_set_gate(14, (u32)_isr14);
	idt_set_gate(15, (u32)_isr15);
	idt_set_gate(16, (u32)_isr16);
	idt_set_gate(17, (u32)_isr17);
	idt_set_gate(18, (u32)_isr18);
	idt_set_gate(19, (u32)_isr19);
	idt_set_gate(20, (u32)_isr20);
	idt_set_gate(21, (u32)_isr21);
	idt_set_gate(22, (u32)_isr22);
	idt_set_gate(23, (u32)_isr23);
	idt_set_gate(24, (u32)_isr24);
	idt_set_gate(25, (u32)_isr25);
	idt_set_gate(26, (u32)_isr26);
	idt_set_gate(27, (u32)_isr27);
	idt_set_gate(28, (u32)_isr28);
	idt_set_gate(29, (u32)_isr29);
	idt_set_gate(30, (u32)_isr30);
	idt_set_gate(31, (u32)_isr31);
	tty_write("[ OK ]\n");
}

char* exception_msg[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void fault_handler(struct regs *r) {
	/* Is this a fault whose number is from 0 to 31? */
    if (r->int_no < 32) {
			tty_write("Exception: ");
			tty_write(exception_msg[r->int_no]);

      for (;;);
    }
}