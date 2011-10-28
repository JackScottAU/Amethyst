/*
	Handler.c - Generic ISR Handler.
	Part of the Jane Microkernel.
	
	Maintainer:	Jack Scott (yayyak@yayyak.net).
	Last Modified:	2006-01-18.
	License:	3-clause BSD License.
	Notes:		None.
*/

#include <portIO.h>
#include <Clock.h>
#include <Registers.h>
#include <interrupts.h>


void interrupts_handler(struct Registers_S *Registers)
{
	/*
	SYNOPSIS:	Takes control of the interrupt and delegates the task.
	INPUT:		Structure of the registers (used for exceptions).
	OUTPUT:		None.
	NOTES:		This is a fairly large function, but it deals with all
			cases. Eventually this function will send a message off
			to another task to actually do the work, but since I
			don't have messaging implemented, some work is done here
			instead.
	*/
	
	//Variable Declarations.
	//unsigned char temp = 0;
	
	//If it is an exception.
	if(Registers->IntNum <= 0x1F)
	{		
		__asm__("cli");
		__asm__("hlt");
	}
	
	//If it is an IRQ.
	if((Registers->IntNum >= 0x20) && (Registers->IntNum <= 0x2F))
	{
		switch(Registers->IntNum)
		{
		case 0x20:	//Programmable Interval Timer
			clock_handler_PIC();
			break;
		
		case 0x21:	//Keyboard
			//temp = readByte(0x60);
			break;
		
		case 0x24:	//COM1
			break;
		
		default:	//As yet unhandled things.
			break;
		}
		
		//Send EOI to 8259 controller.
		if(Registers->IntNum >= 0x28)
		{
			//If it is the slave, send it to that.
			writeByte(0xA0, 0x20);
		}
		
		//But send it to the master no matter what.
		writeByte(0x20, 0x20);
	}
	
	//Kernel Service Interrupt.
	if((Registers->IntNum >= 0x30) && (Registers->IntNum <= 0x31))
	{
		//
	}
}
