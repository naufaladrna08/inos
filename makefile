ARCH ?= x86

CROSS := $(HOME)/opt/cross/bin
CC    := gcc
AS    := $(CROSS)/i686-elf-as
LD    := $(CROSS)/i686-elf-ld
OBJCOPY := $(CROSS)/i686-elf-objcopy

# global containers
obj-y :=
inc-y :=
bin-y :=

export CC AS LD OBJCOPY ARCH obj-y inc-y bin-y

# include arch config
include arch/$(ARCH)/makefile

# kernel build
# include kernel/Makefile

CFLAGS := -ffreestanding -fno-builtin -nostdlib -m32 \
          -fno-stack-protector -O0 -no-pie \
          $(addprefix -I,$(inc-y))

all: boot.img kernel.elf os.img

kernel.elf: $(obj-y)
	$(LD) -T arch/$(ARCH)/linker/link.ld -o $@ $^

os.img: boot.img kernel.elf
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=boot.img of=$@ conv=notrunc
	dd if=kernel.elf of=$@ seek=1 conv=notrunc

clean:
	rm -f $(obj-y) $(bin-y) kernel.elf os.img
