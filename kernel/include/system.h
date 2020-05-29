/*
 * InOS System Header
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

#ifndef SYSTEM_H
#define SYSTEM_H
#include <types.h>
#include <stdlib.h>

/* Kernel Configuration */
#define KERNEL_VERSION_MAJOR  1
#define KERNEL_VERSION_PATCH  0
#define KERNEL_VERSION_MINOR  0

struct regs {
    u32 gs, fs, es, ds;      /* pushed the segs last */
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    u32 int_no, err_code;    /* our 'push byte #' and ecodes do this */
    u32 eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

typedef void (*irq_handler_t) (struct regs *);
typedef int (TASK_ENTRY_POINT_T) (void *);

#endif