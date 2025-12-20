#include <drivers/disk/floppy.h>

static volatile int floppy_irq = 0;

void floppy_irq_handler(struct regs *r) {
  (void) r;
  floppy_irq = 1;
  irq_ack(FLOPPY_IRQ);
}

void floppy_wait_irq() {
  tty_printf("Waiting for IRQ: %d\n", floppy_irq);
  while (!floppy_irq);
  floppy_irq = 0;
}

void floppy_write_dor(u8 value) {
  outb(FLOPPY_DOR, value);
}

u8 floppy_read_status() {
  return inb(FLOPPY_MSR);
}

void floppy_send_command(u8 command) {
  while (!(floppy_read_status() & 0x80));
  outb(FLOPPY_FIFO, command);
}

u8 floppy_read_data() {
  while (!(floppy_read_status() & 0x80));
  return inb(FLOPPY_FIFO);
}

int floppy_check_status() {
  u8 status = floppy_read_status();
  if (status & 0xC0) {
    return 1;
  }
  return 0;
}

void floppy_reset() {
  tty_printf("Resetting floppy drive...\n");
  floppy_write_dor(0x00); // Disable motor and reset
  floppy_write_dor(0x0C); // Enable IRQ and DMA
  floppy_wait_irq();
  tty_printf("Floppy drive reset complete.\n");
}

void floppy_init() {
  irq_install_handler(FLOPPY_IRQ, floppy_irq_handler);

  floppy_reset();
  floppy_motor_off();
}

void floppy_motor_on() {
  u8 dor = 0x1C;
  floppy_write_dor(dor);
}

void floppy_motor_off() {
  u8 dor = 0x0C;
  floppy_write_dor(dor);
}

int floppy_read_sector(u8 cylinder, u8 head, u8 sector, void* buffer) {
  if (!floppy_seek(cylinder, head)) return 0; // Seek failure

  for (int attempt = 0; attempt < MAX_RETRIES; attempt++) {
    floppy_motor_on();
    tty_printf("Motor on\n");
    
    // Send read command
    floppy_send_command(0x46); // Read Data command
    floppy_send_command((head << 2) | 0); // Drive 0, head selection
    floppy_send_command(cylinder); // Cylinder number
    floppy_send_command(head); // Head number
    floppy_send_command(sector); // Sector number
    floppy_send_command(2); // Bytes per sector (512 bytes)
    floppy_send_command(18); // Last sector (18 sectors per track)
    floppy_send_command(0x1B); // Gap length
    floppy_send_command(0xFF); // Data length (ignored for 512 bytes)

    tty_printf("Command sent\n");
    
    floppy_wait_irq(); // Wait for IRQ

    tty_printf("IRQ received\n");
    
    if (floppy_check_status()) {
      for (int i = 0; i < 512; i++) {
        ((u8*) buffer)[i] = floppy_read_data();
      }

      floppy_motor_off();
      return 1; // Success
    }
  }

  floppy_motor_off();
  return 0;
}

int floppy_write_sector(u8 cylinder, u8 head, u8 sector, void* buffer) {
  if (!floppy_seek(cylinder, head)) return 0; // Seek failure
  
  for (int attempt = 0; attempt < MAX_RETRIES; attempt++) {
    floppy_motor_on();
    
    // Send write command
    floppy_send_command(0x45); // Write Data command
    floppy_send_command((head << 2) | 0); // Drive 0, head selection
    floppy_send_command(cylinder); // Cylinder number
    floppy_send_command(head); // Head number
    floppy_send_command(sector); // Sector number
    floppy_send_command(2); // Bytes per sector (512 bytes)
    floppy_send_command(18); // Last sector (18 sectors per track)
    floppy_send_command(0x1B); // Gap length
    floppy_send_command(0xFF); // Data length (ignored for 512 bytes)
    
    // Write data from buffer to FIFO
    for (int i = 0; i < 512; i++) {
      outb(FLOPPY_FIFO, ((u8*) buffer)[i]);
    }
    
    floppy_wait_irq(); // Wait for IRQ
    
    if (floppy_check_status()) {
      floppy_motor_off();
      return 1; // Success
    }
  }

  floppy_motor_off();
  return 0; // Failure after retries
}

int floppy_seek(u8 cylinder, u8 head) {
  tty_printf("Seeking to cylinder %d, head %d...\n", cylinder, head);
  for (int attempt = 0; attempt < MAX_RETRIES; attempt++) {
    floppy_send_command(0x0F); // Seek command
    floppy_send_command((head << 2) | 0); // Drive 0, head selection
    floppy_send_command(cylinder); // Cylinder number
    floppy_wait_irq(); // Wait for completion
    if (floppy_check_status()) {
      tty_printf("Seek successful.\n");
      return 1; // Success
    }
    tty_printf("Seek failed, retrying...\n");
  }
  tty_printf("Seek failed after %d retries.\n", MAX_RETRIES);
  return 0; // Failure after retries
}