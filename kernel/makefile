
# arch-level include dir
inc-y += kernel/include

# bootloader variables
kernel_OBJS := kernel/init/entry.o kernel/init/main.o kernel/tty.o \
	kernel/string.o kernel/io.o

kernel/%.o: kernel/%.c
	$(CC) ${CFLAGS} -c $< -o $@

kernel/%.o: kernel/%.s
	$(AS) --32 -c $< -o $@

kern: $(kernel_OBJS)
	$(LD) -T arch/${ARCH}/linker/kernel.ld -nostdlib -o kern.elf $^
	$(OBJCOPY) -O binary kern.elf kern
	
bin-y += kern kern.elf
obj-y += $(kernel_OBJS)