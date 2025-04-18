#build 
pmn b x86_64-kernel
make all

#make efi file
x86_64-elf-objcopy -I elf64-x86-64 -O efi-app-x86_64 kernel.elf ../myBootLoader/kernel.efi
