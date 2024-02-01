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
#include <Graphics/canvas.h>
#include <Graphics/screenfont.h>


//To shut GCC up.
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData);

void vga_putPixel(Canvas* canvas, uint32 x, uint32 y, uint32 colour);
void vga_drawRect(Canvas* canvas, uint16 x, uint16 y, uint16 w, uint16 h, uint32 colour);
void vga_drawChar(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, char a);
void vga_drawWord(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, char* a);

void vga_putPixel(Canvas* canvas, uint32 x, uint32 y, uint32 colour) {
	uint32* screen = canvas->framebuffer;
	screen[(y * canvas->width) + x] = colour;
}

void vga_drawRect(Canvas* canvas, uint16 x, uint16 y, uint16 w, uint16 h, uint32 colour) {
	for(uint16 i = 0; i < h; i++) {
		for(uint16 j = 0; j < w; j++) {
			vga_putPixel(canvas, x + j, y + i, colour);
		}
	}
}

void vga_drawChar(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, char a) {
	// go to top level.
	uint8* charData = font->characterData;
	charData += a * font->header->height;

	for(int r = 0; r < font->header->height; r++) {
		// draw each row.

		for(int c = 0; c < font->header->width; c++) {
			// draw each column/pixel.
			char yes = (charData[r] << c) & 0x80;

			if(yes) {
				vga_putPixel(canvas, x + c, y + r, colour);
			}
		}
	}
}

void vga_drawWord(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, char* a) {
	int i = 0;

	while(a[i]) {
		vga_drawChar(canvas, font, x + (i*font->header->width), y, colour, a[i]);
		i++;
	}
}

/**
 * Initialises the core systems of the kernel and language runtime before launching a command interpreter.
 * @param magicNumber The check number passed from the multiboot loader.
 * @param multibootData The info data structure passed from the multiboot loader.
 */
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData)
{
	if(magicNumber != 0x2BADB002)
	{
		vgaConsole_printf("Multiboot error found. Halting...");
		interrupts_disableInterrupts();
		haltCPU();
	}
 
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

	vgaConsole_printf("Setting up the keyboard...\t\t\t\t\t\t");
	keyboard_registerHandler();
	vgaConsole_printf("%s", 1);
	
	serial_init(SERIAL_COM1, SERIAL_BAUD_38400);
	serial_writeLine("Synergy OS.");
	
	vgaConsole_printf("Setting up the clock...\t\t\t\t\t\t\t");
	clock_init();
	vgaConsole_printf("%s",1);

	deviceTree_build();

	deviceTree_print(vgaConsole_putChar, false);

	stream_printf(serial_writeChar, "Framebuffer address: %h\n", multibootData->framebuffer_addr);
	stream_printf(serial_writeChar, "Framebuffer pitch: %h\n", multibootData->framebuffer_pitch);
	stream_printf(serial_writeChar, "Framebuffer width: %h\n", multibootData->framebuffer_width);
	stream_printf(serial_writeChar, "Framebuffer height: %h\n", multibootData->framebuffer_height);
	stream_printf(serial_writeChar, "Framebuffer bpp: %h\n", multibootData->framebuffer_bpp);
	stream_printf(serial_writeChar, "Framebuffer type: %h\n", multibootData->framebuffer_type);

	stream_printf(serial_writeChar, "Module start: %h\n", multibootData->modsAddr->start);
	stream_printf(serial_writeChar, "Module end: %h\n", multibootData->modsAddr->end);

	Canvas* canvas = memoryManager_allocate(sizeof(Canvas));
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
	vga_drawWord(canvas, font, 204, 240, 0xCCFFCC, "Ah... but you have seen it.");
	
	while(1)
	{
		serial_writeString("# ");

		// This is the beginning of the shell. In the future we will do things.
		char* entered = serial_readLine();
		vgaConsole_putString(entered);

		if(string_compare(entered, "Get-Time") == 0) {
			stream_printf(serial_writeChar, "Time: %h\n",clock_uptime());
		}
		
		if(string_compare(entered, "Get-DeviceTree") == 0) {
			deviceTree_print(serial_writeChar, false);
		}
	};
}
