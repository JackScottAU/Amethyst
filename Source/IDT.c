/*
	IDT.c - Interrupt Descriptor Table Management.
	Part of the Jane Microkernel.
	
	Maintainer:	Jack Scott (yayyak@yayyak.net).
	Last Modified:	2007-01-18.
	License:	3-clause BSD License.
	Notes:		None.
*/

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
	
	//Empty the IDT.
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
