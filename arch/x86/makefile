 
# arch-level include dir
inc-y += arch/x86/include

include arch/x86/boot/makefile
include arch/x86/setup/makefile

boot.img: boot.bin setup32.bin
	cat $^ > $@

bin-y += boot.img
