#include <portIO.h>
#include <Clock.h>
#include <Registers.h>
#include <interrupts.h>

/**
 * @param Registers The state of the registers on the stack provided by the ISR stub.
 */
void interrupts_handler(struct Registers_S *Registers)
{
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
