#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <types.h>
#include <io.h>

#define KBD_BUFF_SIZE 256

void kbd_push(uint8_t scancode);
void kbd_irq();
void kbd_init();
bool kbd_has_data();
uint8_t kbd_getc();

#endif 