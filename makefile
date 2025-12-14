CC=gcc
AS = nasm
LD = $(HOME)/opt/cross/bin/i686-elf-ld
OD = $(HOME)/opt/cross/bin/i686-elf-objdump

CCFLAGS=-I kernel/include -fno-pie -ffreestanding -m16
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


boot.bin: boot/nasm/boot.asm
	$(AS) boot/nasm/boot.asm -f bin -o boot.bin
	
setup.o: boot/nasm/setup.asm
	nasm boot/nasm/setup.asm -f elf -o setup.o

main.o: boot/nasm/main.c
	$(CC) -c boot/nasm/main.c -o main.o $(CCFLAGS)

setup.bin: setup.o main.o
	$(LD) -m elf_i386 -T linker/link.ld -o setup.elf setup.o main.o
	objcopy -O binary setup.elf setup.bin
	rm setup.elf

# Test only boot.bin
run: boot.bin
	dd if=boot.bin of=disk.img bs=512 count=1 conv=notrunc
	qemu-system-i386 -fda disk.img

# Now test with setup.bin
os.img: boot.bin setup.bin
	dd if=/dev/zero of=os.img bs=512 count=2880
	dd if=boot.bin of=os.img bs=512 count=1 conv=notrunc
	dd if=setup.bin of=os.img bs=512 seek=1 conv=notrunc

runos: os.img
	qemu-system-i386 -fda os.img

clean:
	rm -f *.o *.bin disk.img isoimage.iso