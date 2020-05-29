/*
 * Global Descriptor Table
 * 
 * Distribusi pertama dari kernel inos, `QC Test` untuk pengembangan
 * Notebook Axioo. InOS Quality Control (QC) digunakan untuk menguji 
 * kualitas hardware dari produk komputer seperti pengetesan untuk
 * keyboard, touchpad, kamera, LCD, baterai, dll.
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

#ifndef GDT_H
#define GDT_H
#include <types.h>
#include <tty.h>

extern void gdt_set_gate(int i, u32 base, u32 limit, u8 access, u8 gran);
extern void gdt_install();

#endif /* gdt.h */