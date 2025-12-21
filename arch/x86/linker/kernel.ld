ENTRY(_start)       /* symbol entry point */

SECTIONS {
  . = 0x00100000;

  .text : { *(.text*) }
  .rodata : { *(.rodata*) }
  .data : { *(.data*) }
  .bss : { *(.bss*) }
}
