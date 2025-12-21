setup_OBJS += arch/x86/setup/main.o arch/x86/setup/tty.o arch/x86/setup/idt.o \
	arch/x86/setup/idt_asm.o arch/x86/setup/io.o arch/x86/setup/isr.o \
	arch/x86/setup/pic.o arch/x86/setup/irq.o arch/x86/setup/keyboard.o

arch/x86/setup/%.o: arch/x86/setup/%.c
	$(CC) ${CFLAGS} -c $< -o $@

arch/x86/setup/%.o: arch/x86/setup/%.s
	$(AS) --32 -c $< -o $@

bin-y += setup32.bin
inc-y += arch/x86/setup/include