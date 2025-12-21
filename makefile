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
include kernel/makefile

CFLAGS := -ffreestanding -fno-builtin -nostdlib -m32 \
          -fno-stack-protector -O0 -fno-pie \
          $(addprefix -I,$(inc-y))

all: boot.img kernel.elf os.img

inos: boot kern
	# Pad boot to sector boundary (512 bytes)
	cp boot boot.padded
	truncate -s %512 boot.padded
	cat boot.padded kern > inos
	rm boot.padded

clean:
	rm -f $(obj-y) $(bin-y)

run: inos
	qemu-system-i386 -fda inos