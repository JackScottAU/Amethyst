// Synergy OS

// INCLUDES
//#include "interrupts/ints.h"
//#include "multiboot.h"
#include <Types.h>
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
	
	vgaConsole_putString("Synergy OS!\n");
	vgaConsole_printf("Hex: %h\n",0xCAFEBABE);
	vgaConsole_printf("Status 1: %s\n",1);
	vgaConsole_printf("Status 2: %s\n",0);
	/*	clear_screen(WHITE_TXT);
	k_printf("Welcome to YakOS v0.01", 0, WHITE_TXT);	//Welcome Screen
	k_printf("----------------------", 1, WHITE_TXT);
	update_cursor(1,21);
	
	k_printf("Disabling Interrupts...",2,WHITE_TXT);
	INTS(0);
	
	k_printf("Now Remapping PICs starting at 0x20...",3,WHITE_TXT);
	remap_pics(0x20,0x28);
	
	k_printf("Masking all IRQs",4,WHITE_TXT);
	maskIRQ(0xFF);
	
	k_printf("Filling IDT with Interrupts",5,WHITE_TXT);
	CreateInterrupts();
	
	k_printf("Loading IDT",6,WHITE_TXT);
	loadIDTR();
	
	k_printf("Masking all IRQs",7,WHITE_TXT);
	unmaskIRQ(0xFD);
 * 
 * 	//Init kernel core.
	gdtInstall();
	IDT_Init();
	ISRs_Install();		//} These two could be done in any order.
	PIC_Remap();		//}
	
	//Init subsystems.
	Clock_Init();
	
	//The moment of truth.
	IDT_EnableInts();
	
	k_printf("Enabling Interrupts...",8,WHITE_TXT);
	if(INTS(1))
		k_printf("INTS enabled",9,WHITE_TXT);
	
	k_printf("Now try pressing something on the keyboard...",10,WHITE_TXT); */
	
	while(1)
	{
		//Do Nothing here
	};
}
