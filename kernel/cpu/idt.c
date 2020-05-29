/*
 * Interrupt Descriptor Table
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

#include <cpu/idt.h>
#include <tty.h>
#define KERNEL_CS 0x8E

struct idt_gate_t {
	u16 base_low;
	u16 sel;
	u8  zero;
	u8  flags;
	u16 base_high;
}__attribute__((packed)) ;

struct idt_regs_t {
	u16 limit;
	u32 base;
}__attribute__((packed));

struct idt_gate_t idt[256];
struct idt_regs_t idtr;

void idt_set_gate(int n, u32 handler) {
    idt[n].base_low = (handler & 0xFFFF);
    idt[n].sel = 8;
    idt[n].zero = 0;
    idt[n].flags = 0x8E | 0x60;
    idt[n].base_high = (handler >> 16) & 0xFFFF;
}

/* Implemented in boot.S (Assembly) */ 
extern void idt_load();

void idt_install() {
	tty_write("Setting up Interrupt Descriptor Table... ");
    idtr.base = (u32) &idt;
    idtr.limit = (sizeof(struct idt_gate_t) * 256) - 1;
    idt_load();
    tty_write("[ OK ]\n");
}
