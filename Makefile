# Toolchain:
CC	:= i586-elf-gcc
AS	:= i586-elf-gcc
LD	:= i586-elf-gcc
MKISOFS	:= genisoimage

#Options:
WARNINGS	:= -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wno-discarded-qualifiers -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wuninitialized -Wno-unused-parameter
CFLAGS		:= -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -I Source/Includes -std=c99 $(WARNINGS)

#Top-level targets:
.DEFAULT_GOAL := x86_32
.PHONY: all clean cd-image hdd-image floppy-image lint qemu x86_32 kernel-x86_32 image-x86_32 qemu-x86_32 resources

clean:
	-@rm -rf Build
	-@rm -rf Source/*.o
	-@rm -rf Source/drivers/*.o
	-@rm -rf Source/arch/x86_32/*.o
	-@rm -f  Synergy-OS.iso

x86_32: clean qemu-x86_32

kernel-x86_32: Build/kernel32

image-x86_32: cd-image

lint:
	@cpplint --quiet --recursive --linelength=120 --filter=-readability/casting --root=Source Source

qemu-x86_32: image-x86_32
	@qemu-system-i386 -cpu pentium -m 16 -no-reboot -drive format=raw,media=cdrom,file=Synergy-OS.iso -vga std -serial stdio

cd-image: Build/kernel32 resources
	-@grub-mkrescue -o Synergy-OS.iso Build -quiet

resources:
	-@mkdir -p Build/boot/grub
	@cp Resources/grub.cfg Build/boot/grub
	@cp Resources/Fonts Build -r

#Custom file build targets:
Build/kernel32: Source/kernel.o Source/arch/x86_32/entry.o Source/drivers/pci/deviceNames.o Source/arch/x86_32/rootDevice.o Source/stream.o Source/deviceTree.o Source/arch/x86_32/portIO.o Source/drivers/pciBus.o Source/drivers/keyboard.o Source/drivers/serial.o Source/drivers/vgaConsole.o Source/arch/x86_32/gdt.o Source/arch/x86_32/interrupts_setup.o Source/arch/x86_32/interrupts_handler.o Source/arch/x86_32/interrupts_stubs.o Source/Clock.o Source/memoryManager.o Source/string.o
	-@mkdir -p Build
	@$(LD) -T Resources/Linker-Script.ld -ffreestanding -nostdlib -lgcc -o $@ $^

# Object-file compilation rules:
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	@$(AS) $(CFLAGS) -c $< -o $@
