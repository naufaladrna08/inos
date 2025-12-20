obj-y += \
	arch/x86/boot/boot.o \
	arch/x86/boot/pm.o

arch/x86/boot/boot.o: arch/x86/boot/boot.s
	$(AS) --32 -c $< -o $@

arch/x86/boot/pm.o: arch/x86/boot/pm.s
	$(AS) --32 -c $< -o $@

boot.bin: arch/x86/boot/boot.o arch/x86/boot/pm.o
	$(LD) -Ttext 0 --oformat binary -o $@ $^

debug: boot.bin
# 	$(LD) -m elf_i386 -Ttext 0x7C00 -o boot.elf $^
	qemu-system-i386 \
	  -fda boot.bin \
		-S -s

bin-y += boot.bin
