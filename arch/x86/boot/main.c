#include <tty.h>

void write_string( int colour, const char *string ) {
  volatile char *video = (volatile char*)0xB8000;
  while( *string != 0 ) {
    *video++ = *string++;
    *video++ = colour;
  }
}

int main(void) {
  write_string(0x0F, "Hello Fines hi 1gh dt");
  
  for (;;);
}