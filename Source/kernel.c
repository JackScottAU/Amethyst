// Synergy OS

// INCLUDES
//#include "interrupts/ints.h"
//#include "multiboot.h"
#include <Types.h>
#include <GDT.h>
#include <vgaConsole.h>

//To shut GCC up.
void kernel_initialise(uint32 magicNumber, void* multibootData);

// KERNEL MAIN FUNCTION
void kernel_initialise(uint32 magicNumber, void* multibootData)
{
	if(magicNumber != 0x2BADB002)
	{
		//Print error message.
		//CLI/HLT.
	}
 
	vgaConsole_clearScreen();
	
	vgaConsole_putString("Synergy is booting...\n\n");
	
	vgaConsole_printf("Loading GDT...\t\t\t\t\t\t\t\t");
	gdt_install();
	vgaConsole_printf("%s",1);
	
	vgaConsole_printf("Hex: %h\n",0xCAFEBABE);
	vgaConsole_printf("Status 1: %s\n",1);
	vgaConsole_printf("Status 2: %s\n",0);

	/*INTS(0);
	
	remap_pics(0x20,0x28);
	
	maskIRQ(0xFF);
	
	CreateInterrupts();
	
	loadIDTR();
	
	unmaskIRQ(0xFD);

	//Init kernel core.
	gdtInstall();
	IDT_Init();
	ISRs_Install();		//} These two could be done in any order.
	PIC_Remap();		//}
	
	//Init subsystems.
	Clock_Init();
	
	//The moment of truth.
	IDT_EnableInts();
	
	INTS(1);*/
	
	while(1)
	{
		//Do Nothing here
	};
}
