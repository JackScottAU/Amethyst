//main.c - Kernel initilisation

//Copyright (c) 2006-2008, Jack Scott (jack@jackscott.org)
//Released under the 3-clause BSD License (see documentation/license.txt)

#include <Clock.h>
#include <config.h>
#include <gdt.h>
#include <IDT.h>
#include <Multiboot.h>

void Kernel_Startup(MB_Info_T *MB_Info)
{
	/*
	SYNOPSIS:	Takes control from assembly routine and boots kernel.
	INPUTS:		Multiboot data structure, Multiboot magic checksum.
	OUTPUT:		None. Should not return.
	NOTES:		None.
	*/
	
	//Variable Declarations.
	//int La;
	
	//Init kernel core.
	gdtInstall();
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
