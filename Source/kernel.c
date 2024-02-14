/**
 * kernel.c - Kernel initialisation functions.
 * Part of the Synergy operating system.
 * Licensed under the ISC license.
 */

#include <Clock.h>
#include <GDT.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <multiboot.h>
#include <portIO.h>
#include <Types.h>
#include <vgaConsole.h>
#include <pciBus.h>
#include <keyboard.h>
#include <serial.h>
#include <stream.h>
#include <string.h>
#include <deviceTree.h>
#include <ps2controller.h>

//To shut GCC up.
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData);

/**
 * Initialises the core systems of the kernel and language runtime before launching a command interpreter.
 * @param magicNumber The check number passed from the multiboot loader.
 * @param multibootData The info data structure passed from the multiboot loader.
 */
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData)
{
	if(magicNumber != MULTIBOOT_MAGIC_NUMBER)
	{
		vgaConsole_printf("Multiboot error found. Halting...");
		interrupts_disableInterrupts();
		haltCPU();
	}
 
	vga_initialise();

	vgaConsole_clearScreen();
	
	vgaConsole_printf("Synergy is booting...\n\n");
	
	vgaConsole_printf("Multiboot Flags: \t%h\n",multibootData->flags);
	
	vgaConsole_printf("Loading a GDT...\t\t\t\t\t\t\t");
	gdt_install();
	vgaConsole_printf("%s",1);
	
	vgaConsole_printf("Setting up interrupts...\t\t\t\t\t\t");
	interrupts_initialise();
	vgaConsole_printf("%s",1);
	
	vgaConsole_printf("Setting up the memory manager...\t\t\t\t\t");
	memoryManager_init(multibootData->memoryMapAddress, multibootData->memoryMapLength, (uint32) memoryManager_findEndOfReservedMemory(multibootData->modsAddr, multibootData->modsCount));
	vgaConsole_printf("%s",1);
	
	vgaConsole_printf("Enumerating PCI buses...\t\t\t\t\t\t");
	pci_enumerateBuses();
	vgaConsole_printf("%s",1);
	pci_printBuses();

	
	serial_init(SERIAL_COM1, SERIAL_BAUD_38400);
	serial_writeLine("Synergy OS.");
	
	vgaConsole_printf("Setting up the clock...\t\t\t\t\t\t\t");
	clock_init();
	vgaConsole_printf("%s",1);

	ps2controller_initialise();
	deviceTree_build();

//	deviceTree_print(vgaConsole_putChar, false);

	stream_printf(serial_writeChar, "Framebuffer address: %h\n", multibootData->framebuffer_addr);
	stream_printf(serial_writeChar, "Framebuffer pitch: %h\n", multibootData->framebuffer_pitch);
	stream_printf(serial_writeChar, "Framebuffer width: %h\n", multibootData->framebuffer_width);
	stream_printf(serial_writeChar, "Framebuffer height: %h\n", multibootData->framebuffer_height);
	stream_printf(serial_writeChar, "Framebuffer bpp: %h\n", multibootData->framebuffer_bpp);
	stream_printf(serial_writeChar, "Framebuffer type: %h\n", multibootData->framebuffer_type);

	stream_printf(serial_writeChar, "Module start: %h\n", multibootData->modsAddr->start);
	stream_printf(serial_writeChar, "Module end: %h\n", multibootData->modsAddr->end);

/*	Canvas* canvas = memoryManager_allocate(sizeof(Canvas));
	canvas->framebuffer = (void*)multibootData->framebuffer_addr;
	canvas->height = multibootData->framebuffer_height;
	canvas->width = multibootData->framebuffer_width;

	ScreenFont* font = memoryManager_allocate(sizeof(ScreenFont));
	font->header = multibootData->modsAddr->start;
	font->characterData = (uint8*)(font->header) + font->header->headerSize;

	stream_printf(serial_writeChar, "header: %h\n", font->header);
	stream_printf(serial_writeChar, "header size: %h\n", font->header->headerSize);
	stream_printf(serial_writeChar, "char data: %h\n", font->characterData);

	vga_drawRect(canvas, 200, 200, 500, 300, 0x00888888);

	vga_drawRect(canvas, 204, 224, 500 - 8, 300 - 28, 0x008888FF);
	vga_drawRect(canvas, 200 + 500 - 20, 200 + 4, 16, 16, 0x00444444);
	
	vga_drawWord(canvas, font, 204, 204, 0xCCCCFF, "Synergy OS");

	vga_drawWord(canvas, font, 204, 224, 0xCCCCFF, "This is by far the worst operating system you've ever seen.");
	vga_drawWord(canvas, font, 204, 240, 0xCCFFCC, "Ah... but you have seen it.");*/
	
	while(1)
	{
		stream_printf(vgaConsole_putChar, "> ");

		char* line = stream_readLine(true);

		if(string_compare(line, "Get-DeviceTree") == 0) {
			deviceTree_print(vgaConsole_putChar, true);
			continue;
		}
		
		if(string_compare(line, "Get-Time") == 0) {
			stream_printf(vgaConsole_putChar, "Time: %h\n", clock_uptime());
			continue;
		}

		// None of the built-in commands match the input. 
		stream_printf(vgaConsole_putChar, "Unknown command.\n");
	};
}
