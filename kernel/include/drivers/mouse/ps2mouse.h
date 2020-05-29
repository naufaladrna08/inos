#ifndef PS2MOUSE_H
#define PS2MOUSE_H
#include <io.h>
#include <tty.h>
#include <system.h>
#include <cpu/irq.h>

static void mouse_handler(struct regs *r);
void mouse_install();

#endif