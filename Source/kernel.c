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


//To shut GCC up.
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData);


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
