/*
 * Boot setup with GRUB
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#define ASM_FILE  1
#include "../include/multiboot.h"

/* C symbol format. HAVE_ASM_USCORE is defined by configure. */
#ifdef HAVE_ASM_USCORE
# define EXT_C(sym)                     _ ## sym
#else
# define EXT_C(sym)                     sym
#endif

#ifdef __ELF__
# define AOUT_KLUDGE 0
#else
# define AOUT_KLUDGE MULTIBOOT_AOUT_KLUDGE
#endif

#define MULTIBOOT_HEADER_FLAGS  MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | AOUT_KLUDGE

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
 
  .text
  .globl start, _start

start:
_start:
  jmp multiboot_entry

  .align 4

multiboot_header:
  .long MULTIBOOT_HEADER_MAGIC
  .long MULTIBOOT_HEADER_FLAGS
  .long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

#ifndef __ELF__
  /* header_addr */
  .long   multiboot_header
  /* load_addr */
  .long   _start
  /* load_end_addr */
  .long   _edata
  /* bss_end_addr */
  .long   _end
  /* entry_addr */
  .long   multiboot_entry
#else /* ! __ELF__ */
  .long   0
  .long   0
  .long   0
  .long   0
  .long   0       
#endif /* __ELF__ */
        
multiboot_entry:
  mov $stack_top, %esp

  pushl $0
  popf

  pushl   %ebx
  pushl   %eax

  
  cli
  call kernel_init

1:hlt
	jmp 1b
 
.size _start, . - _start
