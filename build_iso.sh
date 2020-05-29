#!/bin/bash

echo "Creating kernel.elf"
make kernel.elf

echo "Copying kernel.elf"
mv kernel.elf build/boot

echo "Executing grub-mkrescue"
grub-mkrescue --output=./isoimage.iso build