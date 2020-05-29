/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
 
/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	.text 0x1000 : AT(0x1000) {
		code = .;
		*(.text)
		*(.rodata)
		. = ALIGN(4096);
	} 

	.data : AT(0x1000 + (data - code)) {
		data = .;
		*(.data)
		. = ALIGN(4096);
	}

	.bss : AT(0x1000 + (bss - code)) {
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

	end = .;

	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame)
	}
}