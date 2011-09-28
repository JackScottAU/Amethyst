// YakOS -  Main()

// INCLUDES
#include <video.h>
#include <data_io.h>
#include "version.h"
#include "interrupts/ints.h"
#include "multiboot.h"

// KERNEL MAIN FUNCTION
void k_main()
{
	clear_screen(WHITE_TXT);
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
	
	k_printf("Enabling Interrupts...",8,WHITE_TXT);
	if(INTS(1))
		k_printf("INTS enabled",9,WHITE_TXT);
	
	k_printf("Now try pressing something on the keyboard...",10,WHITE_TXT);
	
	while(1)
	{
		//Do Nothing here
	};
};
