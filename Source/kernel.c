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
	
	
	
	

	

	//Init kernel core.
	IDT_Init();
	ISRs_Install();		//} These two could be done in any order.
	idt_remapPICs(0x20);		//}
	vgaConsole_printf("Status 1: %s\n",1);
	
	//Init subsystems.
	clock_init();
	vgaConsole_printf("Status 2: %s\n",0);
	
	//The moment of truth.
	idt_enableInts();
	vgaConsole_printf("Hex: %h\n",0xCAFEBABE);
	
	while(1)
	{
		//Do Nothing here
	};
}
