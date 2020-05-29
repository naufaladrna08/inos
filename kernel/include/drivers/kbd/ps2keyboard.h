#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <types.h>
#include <system.h>
#include <cpu/isr.h>
#include <cpu/irq.h>
#include <io.h>
#include <system.h>
#include <tty.h>
#include <kernel.h>

void keyboard_install();

#endif /* keyboard.h */