CC=gcc
# AS = nasm
AS = $(HOME)/opt/cross/bin/i686-elf-as
LD = $(HOME)/opt/cross/bin/i686-elf-ld
OD = $(HOME)/opt/cross/bin/i686-elf-objdump

CCFLAGS=-Ikernel/include -m32 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -O0 -fno-pic -fno-pie -no-pie
ASFLAGS=--32

OBJECTS=boot/grub/boot.o kernel/io.o kernel/string.o kernel/tty.o kernel/cpu/gdt.o \
				kernel/cpu/gdt_flush.o kernel/cpu/idt.o kernel/cpu/isr.o kernel/cpu/irq.o \
				kernel/cpu/interrupt.o kernel/stdlib.o kernel/drivers/mouse/ps2mouse.o \
				kernel/drivers/kbd/ps2keyboard.o kernel/cpu/reboot.o kernel/init/main.o 

%.o: %.S
	$(AS) $(ASFLAGS) -c $^ -o $@

%.o: %.asm
	nasm $^ -f elf -o $@

%.o: %.c
	$(CC) -c $^ -o $@ $(CCFLAGS)

kernel.elf: $(OBJECTS)
	$(LD) -o kernel.elf -T link.ld $^

runiso: isoimage.iso
	bash ./build_iso.sh
	(killall VirtualBox) || true
	VirtualBox --startvm kernel

boot.o: boot/boot.s
	$(AS) --32 -c boot/boot.s -o boot.o
	
setup.o: boot/setup.s
	$(AS) --32 -c boot/setup.s -o setup.o

boot.bin: boot.o
	$(LD) -m elf_i386 -Ttext 0x7C00 --oformat binary boot.o -o boot.bin

setup.elf: setup.o boot/main.o boot/tty.o
	$(LD) $(LDFLAGS) -Tlinker/link.ld $^ -o setup.elf


check-symbols: setup.elf
	nm setup.elf | grep -E '(main|_start)'
	objdump -d setup.elf | grep -A 10 '<main>'

setup.bin: setup.elf
	objcopy -O binary setup.elf setup.bin

os.img: boot.bin setup.bin
	dd if=/dev/zero of=os.img bs=512 count=2880
	dd if=boot.bin of=os.img bs=512 count=1 conv=notrunc
	dd if=setup.bin of=os.img bs=512 seek=1 conv=notrunc

run: os.img
# 	qemu-system-i386 -fda os.img
	qemu-system-i386 -drive format=raw,file=os.img -d cpu_reset -no-reboot

clean:
	rm -f *.o *.bin disk.img isoimage.iso os.img