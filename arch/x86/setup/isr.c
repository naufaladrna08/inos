#include <isr.h>

void isr_handler(uint32_t int_no, uint32_t err_code) {
  // Simple VGA text output
  volatile char *video = (volatile char*) 0xB8000;
  const char *msg = "INT:";
  
  // Print "INT:" at position
  for (int i = 0; i < 4; i++) {
    video[160 + i*2] = msg[i];      // Row 1, character
    video[160 + i*2 + 1] = 0x0C;    // Red text
  }
  
  // Print interrupt number (simple hex display)
  char hex[] = "0123456789ABCDEF";
  video[168] = hex[(int_no >> 4) & 0xF];
  video[169] = 0x0C;
  video[170] = hex[int_no & 0xF];
  video[171] = 0x0C;

  // Halt
  while(1) {
    __asm__ volatile("hlt");
  }
}