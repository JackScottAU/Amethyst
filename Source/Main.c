/*
	Main.c - System initialisation code.
	Part of the Jane Microkernel.
	
	Author:		Jack Scott (yayyak@yayyak.net).
	Last Modified:	2007-01-17.
	License:	3-clause BSD License.
	Notes:		None.
*/

#include <BasicIO.h>
#include <Clock.h>
#include <config.h>
#include <GDT.h>
#include <IDT.h>
#include <Multiboot.h>

void Kernel_Startup(MB_Info_T *MB_Info, unsigned int MB_Magic)
{
	/*
	SYNOPSIS:	Takes control from assembly routine and boots kernel.
	INPUTS:		Multiboot data structure, Multiboot magic checksum.
	OUTPUT:		None. Should not return.
	NOTES:		None.
	*/
	
	//Variable Declarations.
	//int La;
	
	
	//Read in values from Multiboot.
	if(MB_Magic != 0x2BADB002)
	{
		return;
	}
	
	//Init kernel core.
	GDT_Init();
	IDT_Init();
	ISRs_Install();		//} These two could be done in any order.
	PIC_Remap();		//}
	
	//Init subsystems.
	Clock_Init();
	
	//The moment of truth.
	IDT_EnableInts();
	
	//Idle.
	for(;;)
	{
		__asm__("hlt");
	}
	
	//At some point in time, we will return to the asm, and from there call
	//the interpreter. For now, the hlt above does the trick.
}
