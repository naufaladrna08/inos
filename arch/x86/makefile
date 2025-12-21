 
# arch-level include dir
inc-y += arch/x86/include

# bootloader variables
boot_OBJS  := 
setup_OBJS :=

include arch/x86/boot/makefile
include arch/x86/setup/makefile

bootTemp: $(boot_OBJS)
	$(LD) -Ttext 0 --oformat binary -o bootTemp $^

setup: $(setup_OBJS)
	$(LD) -Ttext 0x9000 --oformat binary -o setup $^	
	
boot: bootTemp setup
	cat bootTemp setup > boot
	
bin-y += boot bootTemp setup
obj-y += $(boot_OBJS) $(setup_OBJS)