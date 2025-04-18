#build executable file
build:kernel.elf

#make objs
obj:
	@mkdir obj

obj/kernel.o: init/kernel.c
	x86_64-elf-gcc -o obj/kernel.o init/kernel.c -I . -ffreestanding -fpic -fno-stack-protector -fshort-wchar -mno-red-zone -mgeneral-regs-only -mabi=ms -Wall -Wextra -Wpedantic -O3 -c

kernel.elf: obj obj/kernel.o
	x86_64-elf-gcc -o kernel.elf obj/kernel.o -nostdlib -Wl,-T,x86_64.lds -Wl,-Bsymbolic -Wl,-znocombreloc -lgcc

all: clean kernel.elf

clean:
	$(RM) kernel.elf obj/kernel.o