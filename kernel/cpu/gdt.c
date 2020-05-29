/*
 * Global Descriptor Table
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

#include <cpu/gdt.h>

/*
 * the GDT Entry
 */
struct gdt_entry {
	/* Limits */
	u16 limit_low;
	/* Segment addr */
	u16 base_low;
	u8  base_mid;
	/* Access modes */
	u8  access;
	u8  gran;
	u8  base_high;
}__attribute__((packed));

/*
 * GDT Pointer
 */
struct gdt_ptr {
	u16 limit;
	u32 base;
}__attribute__((packed));

struct gdt_entry gdt[6];
struct gdt_ptr	 gdtr;

void gdt_set_gate(int i, u32 base, u32 limit, u8 access, u8 gran) {
	gdt[i].limit_low = (limit & 0xFFFF);
	gdt[i].base_low  = (base & 0xFFFF);
	gdt[i].base_mid  = (base >> 16) & 0xFF;
	gdt[i].base_high = (base >> 24) & 0xFF;
	gdt[i].gran			 = (limit >> 16) & 0x0F;   
	gdt[i].gran			|= (gran & 0xF0);   
	gdt[i].access 	 = access;
}

/* Implemented in boot.S (Assembly) */
extern void gdt_flush(u32);

void gdt_install() {
	tty_write("Setting up Global Descriptor Table... ");
	gdtr.limit = (sizeof(struct gdt_entry) * 6) - 1;
	gdtr.base  = (u32) &gdt; 
	/* NULL Segment */
	gdt_set_gate(0, 0, 0, 0, 0);
	/* CSEG */
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	/* DSEG */
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	/* User code */
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	/* User data */
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	gdt_flush((u32) &gdtr);

	tty_write("[ OK ]\n");
}