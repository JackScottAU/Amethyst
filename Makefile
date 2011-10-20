# Toolchain:
CC	= /usr/cross/bin/i586-elf-gcc
AS	= /usr/cross/bin/i586-elf-as
LD	= /usr/cross/bin/i586-elf-ld
MKISOFS	= genisoimage

#Options
CFLAGS	= -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -I Source/Includes

clean:
	-@rm -r Build
	-@rm -r Source/*.o

cd-image: Build/kernel32
	@mkdir -p Build/boot/grub
	@cp Resources/stage2_eltorito Build/boot/grub
	@cp Resources/menu.lst Build/boot/grub
	@$(MKISOFS) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o Synergy-OS.iso Build

Build/kernel32: Source/kernel.o Source/entry.o Source/portIO.o
	-@mkdir Build
	@$(LD) -T Resources/Linker-Script.ld -o Build/kernel32 Source/entry.o Source/kernel.o Source/portIO.o

Source/entry.o: Source/entry.S
	@$(AS) -o Source/entry.o Source/entry.S

Source/kernel.o: Source/kernel.c
	@$(CC) -o Source/kernel.o -c Source/kernel.c $(CFLAGS)
	
Source/portIO.o: Source/portIO.c
	@$(CC) -o Source/portIO.o -c Source/portIO.c $(CFLAGS)
	
Source/Video.o: Source/Video.c
	@$(CC) -o Source/Video.o -c Source/Video.c $(CFLAGS)