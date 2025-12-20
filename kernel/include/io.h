#ifndef IO_H
#define IO_H
#include <types.h>

extern u8 inb(u16 _port);
extern u16 inw(u16 _port);
extern u32 inl(u32 _port);
extern void outb(u16 _port, u8 _data);
extern void outw(u16 _port, u16 _data);
extern void outl(u32 _port, u32 _data);
extern void outsw(u16 _port, u16 *_data, u32 _count);

#endif