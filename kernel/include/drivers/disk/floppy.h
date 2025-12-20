#ifndef FLOPPY_H
#define FLOPPY_H
#include <io.h>
#include <types.h>
#include <cpu/irq.h>

#define FLOPPY_DOR 0x3F2 
#define FLOPPY_MSR 0x3F4
#define FLOPPY_FIFO 0x3F5
#define FLOPPY_CCR 0x3F7

#define FLOPPY_IRQ 6
#define MAX_RETRIES 3

void floppy_irq_handler(struct regs *r);
void floppy_wait_irq();
void floppy_write_dor(u8 value);
u8 floppy_read_status();
void floppy_send_command(u8 command);
u8 floppy_read_data();
int floppy_check_status();
void floppy_reset();
void floppy_init();
void floppy_motor_on();
void floppy_motor_off();
int floppy_read_sector(u8 cylinder, u8 head, u8 sector, void* buffer);
int floppy_write_sector(u8 cylinder, u8 head, u8 sector, void* buffer);
int floppy_seek(u8 cylinder, u8 head);

#endif