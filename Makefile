# Toolchain:
CC	= /usr/cross/bin/i586-elf-gcc
AS	= /usr/cross/bin/i586-elf-as
LD	= /usr/cross/bin/i586-elf-ld

kernel32:
	$(LD) -T Resources/Linker-Script.ld -o kernel32 entry.o kernel.o

entry.o:
	$(AS) -o entry.o Source/entry.S

kernel.o:
	$(CC) -o kernel.o -c Source/kernel.c -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs

cd-image: kernel32
	mkdir -p temp/boot/grub
	cp Resources/stage2_eltorito temp/boot/grub
	cp Resources/menu.lst temp/boot/grub
	mv kernel32 temp
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o Synergy-OS.iso temp