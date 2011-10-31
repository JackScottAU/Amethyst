# Toolchain:
CC	:= /usr/cross/bin/i586-elf-gcc
AS	:= /usr/cross/bin/i586-elf-as
LD	:= /usr/cross/bin/i586-elf-ld
MKISOFS	:= genisoimage

#Options:
WARNINGS	:= -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wuninitialized
CFLAGS		:= -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -I Source/Includes -std=c99 $(WARNINGS)

#Top-level targets:
.PHONY: all clean cd-image hdd-image floppy-image

all: clean cd-image

clean:
	-@rm -r Build
	-@rm -r Source/*.o
	-@rm Synergy-OS.iso

cd-image: Build/kernel32
	-@mkdir -p Build/boot/grub
	@cp Resources/stage2_eltorito Build/boot/grub
	@cp Resources/menu.lst Build/boot/grub
	@$(MKISOFS) -quiet -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o Synergy-OS.iso Build

#Custom file build targets:
Build/kernel32: Source/kernel.o Source/entry.o Source/portIO.o Source/vgaConsole.o Source/gdt.o Source/x86/interrupts_setup.o Source/x86/interrupts_handler.o Source/x86/interrupts_stubs.o Source/Clock.o
	-@mkdir -p Build
	@$(LD) -T Resources/Linker-Script.ld -o $@ $^

Source/entry.o: Source/entry.S
	@$(AS) -o Source/entry.o Source/entry.S
	
Source/gdt.o: Source/gdt.asm
	@nasm -f elf -o Source/gdt.o Source/gdt.asm
	
Source/x86/interrupts_stubs.o: Source/x86/interrupts_stubs.asm
	@nasm -f elf -o Source/x86/interrupts_stubs.o Source/x86/interrupts_stubs.asm

# Object-file compilation rules:
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
