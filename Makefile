# Toolchain:
CC	:= i586-elf-gcc
AS	:= i586-elf-as
LD	:= i586-elf-ld
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
	-@rm -r Source/drivers/*.o
	-@rm -r Source/arch/x86/*.o
	-@rm Synergy-OS.iso

cd-image: Build/kernel32
	-@mkdir -p Build/boot/grub
	@cp Resources/stage2_eltorito Build/boot/grub
	@cp Resources/menu.lst Build/boot/grub
	@$(MKISOFS) -quiet -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o Synergy-OS.iso Build

#Custom file build targets:
Build/kernel32: Source/kernel.o Source/arch/x86/entry.o Source/arch/x86/portIO.o Source/drivers/pciBus.o Source/drivers/vgaConsole.o Source/arch/x86/gdt.o Source/arch/x86/interrupts_setup.o Source/arch/x86/interrupts_handler.o Source/arch/x86/interrupts_stubs.o Source/Clock.o Source/memoryManager.o
	-@mkdir -p Build
	@$(LD) -T Resources/Linker-Script.ld -o $@ $^

Source/arch/x86/entry.o: Source/arch/x86/entry.S
	@$(AS) -o Source/arch/x86/entry.o Source/arch/x86/entry.S
	
Source/arch/x86/gdt.o: Source/arch/x86/gdt.S
	@$(AS) -o Source/arch/x86/gdt.o Source/arch/x86/gdt.S
	
Source/arch/x86/interrupts_stubs.o: Source/arch/x86/interrupts_stubs.S
	@$(AS) -o Source/arch/x86/interrupts_stubs.o Source/arch/x86/interrupts_stubs.S

# Object-file compilation rules:
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
