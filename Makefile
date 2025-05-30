# Toolchain:
CC	:= i586-elf-gcc
AS	:= i586-elf-gcc
LD	:= i586-elf-gcc

#Options:
WARNINGS	:= -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wno-discarded-qualifiers -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wuninitialized -Wno-unused-parameter
CFLAGS		:= -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -I Source/Includes -std=c2x $(WARNINGS)
CPPFLAGS	:= -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -I Source/Includes -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti  -include ./Source/Includes/cppsupport.hpp

#Top-level targets:
.DEFAULT_GOAL := x86_32
.PHONY: all clean cd-image hdd-image floppy-image lint qemu x86_32 kernel-x86_32 image-x86_32 qemu-x86_32 resources

clean:
	-@rm -rf Build
	-@find . -name \*.o -type f -delete
	-@rm -f  Amethyst.iso

x86_32: clean qemu-x86_32

kernel-x86_32: Build/kernel32

image-x86_32: cd-image

lint:
	@cpplint --quiet --recursive --linelength=160 --filter=-readability/casting,-build/include_order --root=Source Source

qemu-x86_32: image-x86_32 cd-image
	@qemu-system-i386 \
		-no-reboot -no-shutdown \
		--machine pc -cpu pentium -m 16  \
		-cdrom Amethyst.iso \
		-vga std \
		-serial stdio \
		-device pvpanic

cd-image: Build/kernel32 resources
	-@grub-mkrescue -o Amethyst.iso Build -quiet

disk-image: Build/kernel32 resources
	dd if=/dev/zero of=disk.img bs=512 count=131072
	sudo losetup /dev/loop1 disk.img
	sudo sfdisk --force /dev/loop1 -u S < Resources/diskformat
	sudo sync
	sudo losetup -d /dev/loop1
	sudo sync

# This line is prone to breakage! We assume it goes on loop0, but it could go on any loop device. We need to save it to a variable or something.
	sudo losetup -P -f disk.img --show
	sudo mkdosfs -F32 -f 2 /dev/loop0p1
	sudo mount /dev/loop0p1 /mnt
	sudo grub-install --boot-directory=/mnt/boot --no-floppy /dev/loop0
	sudo cp -r Build/* /mnt
	sudo sync
	sudo umount /mnt
	sudo losetup -d /dev/loop0

disk-image-cleanup:
	sudo umount /mnt
	sudo losetup -d /dev/loop0
	sudo losetup -d /dev/loop1

resources: 
	-@mkdir -p Build/boot/grub
	@cp Resources/grub.cfg Build/boot/grub
	@cp Resources/Fonts Build -r
	@cp Resources/Images Build -r

#Custom file build targets:
Build/kernel32: Source/arch/x86_32/entry.o Source/kernel.o Source/shell.o Source/drivers/qemu_display_driver.o Source/arch/x86_32/physicalMemory.o Source/drivers/piixide.o Source/TargaImage.o Source/Window.o Source/TextConsole.o Source/TextLabel.o Source/drivers/vesa_framebuffer.o Source/StandardIO.o Source/arch/x86_32/taskswitch.o Source/arch/x86_32/thread.o Source/drivers/mouse.o Source/library/memory.o Source/cpuid.o Source/debug.o Source/library/fifobuffer.o Source/drivers/pci/deviceNames.o Source/drivers/ps2controller.o Source/arch/x86_32/rootDevice.o Source/stream.o Source/deviceTree.o Source/arch/x86_32/portIO.o Source/drivers/pciBus.o Source/drivers/keyboard.o Source/drivers/serial.o Source/arch/x86_32/gdt.o Source/arch/x86_32/interrupts_setup.o Source/arch/x86_32/interrupts_handler.o Source/arch/x86_32/interrupts_stubs.o Source/Clock.o Source/memoryManager.o Source/library/string.o
	-@mkdir -p Build
	@$(LD) -T Resources/Linker-Script.ld -ffreestanding -nostdlib -lgcc -o $@ $^

# Object-file compilation rules:
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@$(CC) $(CPPFLAGS) -c $< -o $@

%.o: %.S
	@$(AS) $(CFLAGS) -c $< -o $@

%.o: %.asm
	@nasm -felf32 $< -o $@
