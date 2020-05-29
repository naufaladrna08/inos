CC=gcc
AS=gcc
LD=ld -m elf_i386

CCFLAGS=-I kernel/include -masm=intel -fno-pie -ffreestanding -m32
ASFLAGS=-m32

OBJECTS=boot/grub/boot.o kernel/io.o kernel/string.o kernel/tty.o kernel/cpu/gdt.o \
				kernel/cpu/gdt_flush.o kernel/cpu/idt.o kernel/cpu/isr.o kernel/cpu/irq.o \
				kernel/cpu/interrupt.o kernel/stdlib.o kernel/drivers/mouse/ps2mouse.o \
				kernel/drivers/kbd/ps2keyboard.o kernel/drivers/ata/ata.o kernel/init/main.o 

%.o: %.S
	$(AS) $(ASFLAGS) -c $^ -o $@

%.o: %.asm
	nasm $^ -f elf -o $@

%.o: %.c
	$(CC) -c $^ -o $@ $(CCFLAGS)

kernel.elf: $(OBJECTS)
	$(LD) -o kernel.elf -T link.ld $^

run: kernel.elf
	qemu-system-i386 -kernel kernel.elf

runiso: isoimage.iso
	bash ./build_iso.sh
	(killall VirtualBox) || true
	VirtualBox --startvm kernel

clean:
	rm -rf ${OBJECTS} kernel.elf