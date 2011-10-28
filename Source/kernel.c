// Synergy OS

// INCLUDES
//#include "interrupts/ints.h"
#include <multiboot.h>
#include <Types.h>
#include <GDT.h>
#include <vgaConsole.h>
#include <IDT.h>
#include <Clock.h>

//To shut GCC up.
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData);

// KERNEL MAIN FUNCTION
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData)
{
	if(magicNumber != 0x2BADB002)
	{
		vgaConsole_printf("Multiboot error found. Halting...");
		//CLI/HLT.
	}
 
	vgaConsole_clearScreen();
	
	vgaConsole_printf("Synergy is booting...\n\n");
	
	vgaConsole_printf("Multiboot Flags: \t%h\n",multibootData->flags);
	
	vgaConsole_printf("Loading GDT...\t\t\t\t\t\t\t\t");
	gdt_install();
	vgaConsole_printf("%s",1);
	
	vgaConsole_printf("Setting up Interrupts...\t\t\t\t\t\t");
	interrupts_initialise();
	vgaConsole_printf("%s",1);
	
	clock_init();
	
	while(1)
	{
		//Do Nothing here
	};
}
