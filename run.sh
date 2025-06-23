#!/bin/sh
unset GTK_PATH

mkdir -p isofiles/boot/grub
cp grub.cfg isofiles/boot/grub/
cp kernel.elf isofiles/boot/
grub-mkrescue -o os.iso isofiles/

qemu-system-x86_64 -machine q35 -cpu qemu64 -m 256 -smp 2 -drive format=raw,file=os.iso -d cpu_reset,int,guest_errors -D qemu.log -monitor stdio