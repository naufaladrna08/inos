#ifndef PS2MOUSE_H
#define PS2MOUSE_H
#include <io.h>
#include <tty.h>
#include <system.h>
#include <cpu/irq.h>
#include <types.h>

static void wait_controller_ready();
static int wait_controller_response();
static void mouse_handler(struct regs* r);
int mouse_install();

#endif