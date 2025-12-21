boot_OBJS  += arch/x86/boot/boot.o 
setup_OBJS += arch/x86/boot/pm.o

arch/x86/boot/%.o: arch/x86/boot/%.s
	$(AS) --32 -c $< -o $@