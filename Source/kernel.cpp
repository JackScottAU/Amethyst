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
#include <cppsupport.hpp>

#include "test.hpp"

#ifdef	__cplusplus
extern "C" {
#endif

//To shut GCC up.
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData);
struct multiboot_info* multiboot_correctDataStructureAddresses(struct multiboot_info* data);
void kernel_printBanner(void);

struct multiboot_info* multiboot_correctDataStructureAddresses(struct multiboot_info* data) {
	data = (struct multiboot_info*)(((uint32) data) + 0xC0000000);

	return data;
}

void kernel_printBanner(void) {
	vgaConsole_setColour(VGACONSOLE_MAGENTA, VGACONSOLE_BLACK);

	vgaConsole_printf("\n\t          _                   _   _               _   \n");
	vgaConsole_printf("\t         / \\   _ __ ___   ___| |_| |__  _   _ ___| |_ \n");
	vgaConsole_printf("\t        / _ \\ | '_ ` _ \\ / _ \\ __| '_ \\| | | / __| __|\n");
	vgaConsole_printf("\t       / ___ \\| | | | | |  __/ |_| | | | |_| \\__ \\ |_ \n");
	vgaConsole_printf("\t      /_/   \\_\\_| |_| |_|\\___|\\__|_| |_|\\__, |___/\\__|\n");
	vgaConsole_printf("\t                                        |___/         \n\n");

	vgaConsole_setColour(VGACONSOLE_LIGHT_GREY, VGACONSOLE_BLACK);
}

/**
 * Initialises the core systems of the kernel and language runtime before launching a command interpreter.
 * @param magicNumber The check number passed from the multiboot loader.
 * @param multibootData The info data structure passed from the multiboot loader.
 */
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData)
{
	vgaConsole_initialise();
	vgaConsole_clearScreen();

	kernel_printBanner();
	
	vgaConsole_printf("Checking Multiboot data...\t\t\t\t\t\t");
	if(magicNumber != MULTIBOOT_MAGIC_NUMBER)
	{
		vgaConsole_printf("\nMultiboot error found. Halting...");
		interrupts_disableInterrupts();
		haltCPU();
	} else {
		multibootData = multiboot_correctDataStructureAddresses(multibootData);
	}
	vgaConsole_printf("%s",1);
	
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

	
	serial_init(SERIAL_COM1, SERIAL_BAUD_38400);
	serial_writeLine("Amethyst Debugging Information:");
	
	vgaConsole_printf("Setting up the clock...\t\t\t\t\t\t\t");
	clock_init();
	vgaConsole_printf("%s",1);

	ps2controller_initialise();
	deviceTree_build();

	A2DD foo = A2DD(1,2);
	A2DD foo2 = A2DD(44,4);

	vgaConsole_printf("%d\n", foo.getSum());
	vgaConsole_printf("%d", foo2.getSum());

//	deviceTree_print(vgaConsole_putChar, false);

	stream_printf(serial_writeChar, "Framebuffer address: %h\n", multibootData->framebuffer_addr);
	stream_printf(serial_writeChar, "Framebuffer pitch: %h\n", multibootData->framebuffer_pitch);
	stream_printf(serial_writeChar, "Framebuffer width: %h\n", multibootData->framebuffer_width);
	stream_printf(serial_writeChar, "Framebuffer height: %h\n", multibootData->framebuffer_height);
	stream_printf(serial_writeChar, "Framebuffer bpp: %h\n", multibootData->framebuffer_bpp);
	stream_printf(serial_writeChar, "Framebuffer type: %h\n", multibootData->framebuffer_type);

	//stream_printf(serial_writeChar, "Module start: %h\n", multibootData->modsAddr->start);
	//stream_printf(serial_writeChar, "Module end: %h\n", multibootData->modsAddr->end);

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
	
	vga_drawWord(canvas, font, 204, 204, 0xCCCCFF, "Amethyst OS");

	vga_drawWord(canvas, font, 204, 224, 0xCCCCFF, "This is by far the worst operating system you've ever seen.");
	vga_drawWord(canvas, font, 204, 240, 0xCCFFCC, "Ah... but you have seen it.");*/
	
	stream_printf(vgaConsole_putChar, "\n");

	cpp_entry();

	foo::bar::bob2();

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

		if(string_compare(line, "Shutdown") == 0) {
			stream_printf(vgaConsole_putChar, "Shutting down...\n");
			interrupts_disableInterrupts();
			stream_printf(vgaConsole_putChar, "It is now safe to turn off your PC.");
			haltCPU();
			break;
		}

		if(string_compare(line, "Get-PciDetails") == 0) {
			pci_printBuses();
			continue;
		}

		// None of the built-in commands match the input.
		stream_printf(vgaConsole_putChar, "Unknown command.\n");
	};
}

#ifdef	__cplusplus
}
#endif
