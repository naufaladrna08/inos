/*
 * Interrupt Requests
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

#ifndef IRQ_H
#define IRQ_H
#include <tty.h>
#include <cpu/idt.h>
#include <cpu/isr.h>
#include <io.h> /* outb */ 
#include <system.h>

extern void irq_install_handler(int irq, void (*handler) (struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_remap();
extern void irq_ack(int irq_no);
extern void irq_gates();
extern void irq_install();
extern void irq_handler(struct regs *r);

#endif /* irq.h */