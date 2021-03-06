#ifndef IO_H
#define IO_H
#include <types.h>

extern u8 inb(u16 _port);
extern u16 inw(u16 _port);
extern void outb(u16 _port, u8 _data);
extern void outw(u16 _port, u16 _data);

#endif