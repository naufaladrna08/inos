SETUP_OBJS := \
	arch/x86/setup/idt.o \
	arch/x86/setup/tty.o \
	arch/x86/setup/main.o 
obj-y += ${SETUP_OBJS}

arch/x86/setup/%.o: arch/x86/setup/%.s
	$(AS) --32 -c $< -o $@

arch/x86/setup/%.o: arch/x86/setup/%.c
	$(CC) ${CFLAGS} -c $< -o $@

setup32.elf: ${SETUP_OBJS}
	$(LD) -T arch/x86/linker/link.ld -o $@ $^

setup32.bin: setup32.elf
	$(OBJCOPY) -O binary $< $@

bin-y += setup32.bin
