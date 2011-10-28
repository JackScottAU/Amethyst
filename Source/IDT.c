/*
	IDT.c - Interrupt Descriptor Table Management.
	Part of the Jane Microkernel.
	
	Maintainer:	Jack Scott (yayyak@yayyak.net).
	Last Modified:	2007-01-18.
	License:	3-clause BSD License.
	Notes:		None.
*/

#include <portIO.h>
#include <Types.h>
#include <IDT.h>
#include <Clock.h>

struct IDT_Entry_S
{
	unsigned short Address_Low;
	unsigned short Selector;
	unsigned char Empty;
	unsigned char Flags;
	unsigned short Address_High;
} __attribute__((packed));

struct IDT_Pointer_S
{
	unsigned short Limit;
	unsigned int Base;
} __attribute__((packed));

//Random comment.

struct IDT_Entry_S IDT_Table[256];
struct IDT_Pointer_S IDT_Pointer;


void IDT_SetGate(unsigned char Number, unsigned long Address, unsigned short Selector, unsigned char Flags)
{
	IDT_Table[Number].Address_Low = (Address & 0xFFFF);
	IDT_Table[Number].Address_High = (Address >> 16) & 0xFFFF;
	
	IDT_Table[Number].Selector = Selector;
	IDT_Table[Number].Empty = 0x00;
	IDT_Table[Number].Flags = Flags;
}

void IDT_Init()
{
	/*
	SYNOPSIS:	Clears the IDT, and installs it ready to use.
	INPUTS:		None.
	OUTPUT:		None.
	NOTES:		None.
	*/
	
	unsigned char *Temp;
	unsigned int Count = 0;
	
	//Create the pointer.
	IDT_Pointer.Limit = (sizeof (struct IDT_Entry_S) * 256) - 1;
	IDT_Pointer.Base = (unsigned int) &IDT_Table;
	
	//Empty the IDT. THis is a hack of a code.
	Temp = (void*) &IDT_Table;
	while(Count < (sizeof(struct IDT_Entry_S) * 256))
	{
		*Temp = 0x00;
		Temp++;
		Count++;
	}
	
	//Do the install.
	__asm__ __volatile__("lidt %0"::"m"(IDT_Pointer));
}

#define IDT_PIC1_BASEADDRESS 0x20
#define IDT_PIC2_BASEADDRESS 0xA0

void idt_remapPICs(uint8 baseIRQ)
{
	writeByte(IDT_PIC1_BASEADDRESS, 0x11);
	writeByte(IDT_PIC2_BASEADDRESS, 0x11);
	
	writeByte(IDT_PIC1_BASEADDRESS+1, baseIRQ);
	writeByte(IDT_PIC2_BASEADDRESS+1, baseIRQ+0x08);
	
	writeByte(IDT_PIC1_BASEADDRESS+1, 0x04);
	writeByte(IDT_PIC2_BASEADDRESS+1, 0x02);
	
	writeByte(IDT_PIC1_BASEADDRESS+1, 0x01);
	writeByte(IDT_PIC2_BASEADDRESS+1, 0x01);
	
	writeByte(IDT_PIC1_BASEADDRESS+1, 0x0);
	writeByte(IDT_PIC2_BASEADDRESS+1, 0x0);
}

void interrupts_disableInterrupts(void)
{
	__asm__ volatile("cli");
}

void interrupts_enableInterrupts(void)
{
	__asm__ volatile("sti");
}

void interrupts_initialise(void)
{
	IDT_Init();
	ISRs_Install();		//} These two could be done in any order.
	idt_remapPICs(0x20);		//}
	
	clock_init();
	
	interrupts_enableInterrupts();
}