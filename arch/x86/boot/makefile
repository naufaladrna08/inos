boot_OBJS  += arch/x86/boot/boot.o 
setup_OBJS += arch/x86/boot/pm.o

*.o: %.s
	$(AS) --32 -c $< -o $@