/*
 * Interrupt Requests
 * 
 * Created by: Naufal Adriansyah
 * Thanks to : 
 *  - Dennis Ritchie (1941-2011) sebagai pembuat bahasa C dan Unix
 *  - Nick Blundell, pembuat buku "Write a Simple Operating System
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

#include <cpu/irq.h>

#include <drivers/mouse/ps2mouse.h>

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

void *irq_routines[16] = {
	0, 0, 0, 0, 0, 0 ,0, 0,
	0, 0, 0, 0, 0, 0 ,0, 0
};

/* 
 * Fungsi memasangang IRQ Handler untuk source 
 */
void irq_install_handler(int irq, void (*handler) (struct regs *r)) {
	irq_routines[irq] = handler;	
}

void irq_uninstall_handler(int irq) {
	irq_routines[irq] = 0;
}

void irq_ack(int irq_no) {
	if (irq_no >= 12) {
		outb(0xA0, 0x20);
	}
	
	outb(0x20, 0x20);
}

void irq_remap() {
	tty_write("\nRemapping Interrupt Requests... ");

	// Save masks
	u8 a1 = inb(0xA1);
	u8 a2 = inb(0x21);

	// Start initialization
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20); // Master PIC vector offset
	outb(0xA1, 0x28); // Slave PIC vector offset
	outb(0x21, 0x04); // Tell Master PIC about the Slave at IRQ2
	outb(0xA1, 0x02); // Tell Slave PIC its cascade identity
	outb(0x21, 0x01); // 8086 mode
	outb(0xA1, 0x01); // 8086 mode

	// Restore masks
	outb(0x21, a2);
	outb(0xA1, a1);

	tty_write("[ OK ]\n");
}

void irq_gates() {
	tty_write("Setting up Interrupt Requests Gates... ");
	idt_set_gate(32, (u32)_irq0);
	idt_set_gate(33, (u32)_irq1);
	idt_set_gate(34, (u32)_irq2);
	idt_set_gate(35, (u32)_irq3);
	idt_set_gate(36, (u32)_irq4);
	idt_set_gate(37, (u32)_irq5);
	idt_set_gate(38, (u32)_irq6);
	idt_set_gate(39, (u32)_irq7);
	idt_set_gate(40, (u32)_irq8);
	idt_set_gate(41, (u32)_irq9);
	idt_set_gate(42, (u32)_irq10);
	idt_set_gate(43, (u32)_irq11);
	idt_set_gate(44, (u32)_irq12);
	idt_set_gate(45, (u32)_irq13);
	idt_set_gate(46, (u32)_irq14);
	idt_set_gate(47, (u32)_irq15);

	tty_write("[ OK ]\n");
}

void irq_install() {
	tty_write("Setting up hardware interrupts...");
	irq_remap();
	irq_gates();
	__asm__ __volatile__("sti");
}

void irq_handler(struct regs *r) {
	/* Clear interrupt  */
	asm("cli");

	void (*handler) (struct regs *r);
	if (r->int_no > 47 || r->int_no < 32) {
		handler = NULL;
	} else {
		handler = irq_routines[r->int_no - 32];
	}
	if (handler) {
		handler(r);
	} else {
		irq_ack(r->int_no - 32);
	}

	/* Store */
	asm("sti");
}