#!/bin/sh
unset GTK_PATH

mkdir -p isofiles/boot/grub
cp grub.cfg isofiles/boot/grub/
cp kernel.elf isofiles/boot/
grub-mkrescue -o os.iso isofiles/

if hash qemu-system-x86_64 2>/dev/null; then
	qemu-system-x86_64 -machine q35 -cpu qemu64 -m 10240 -smp 2 -drive format=raw,file=os.iso -d cpu_reset,int,guest_errors -D qemu.log -monitor stdio
fi