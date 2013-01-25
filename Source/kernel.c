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


//To shut GCC up.
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData);
void kernel_testClock(void);

clock_timerRequest* grrr;

int i = 0;

void kernel_testClock(void)
{
	vgaConsole_printf("Clock shot! Time: %h\n",clock_uptime());
	
	i++;
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
	
	vgaConsole_printf("Enumerating PCI buses...\n");
	pci_enumerateBuses();
	
	vgaConsole_printf("Setting up the clock...\t\t\t\t\t\t\t");
	clock_init();
	vgaConsole_printf("%s",1);
	
	grrr = clock_addRepeatRequest(1, 0, (*kernel_testClock));
	
	while(1)
	{
		//Do Nothing here
		//In here we should do things like reordering free memory blocks.
		
		if(i==10)
		{
			clock_deleteTimerRequest(grrr);
			
			i=0;
		}
	};
}

