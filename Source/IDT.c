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
	interrupts_installEmptyIDT();
	interrupts_installISRs();		//} These two could be done in any order.
	interrupts_remap8259s(0x20);		//}
	
	interrupts_enableInterrupts();
}

void interrupts_installEmptyIDT()
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

void interrupts_installISRs()
{
	interrupts_setGate(0x00, (unsigned)interrupts_ISR_00, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x01, (unsigned)interrupts_ISR_01, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x02, (unsigned)interrupts_ISR_02, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x03, (unsigned)interrupts_ISR_03, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x04, (unsigned)interrupts_ISR_04, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x05, (unsigned)interrupts_ISR_05, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x06, (unsigned)interrupts_ISR_06, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x07, (unsigned)interrupts_ISR_07, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x08, (unsigned)interrupts_ISR_08, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x09, (unsigned)interrupts_ISR_09, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x0A, (unsigned)interrupts_ISR_0A, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x0B, (unsigned)interrupts_ISR_0B, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x0C, (unsigned)interrupts_ISR_0C, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x0D, (unsigned)interrupts_ISR_0D, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x0E, (unsigned)interrupts_ISR_0E, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x0F, (unsigned)interrupts_ISR_0F, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x10, (unsigned)interrupts_ISR_10, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x11, (unsigned)interrupts_ISR_11, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x12, (unsigned)interrupts_ISR_12, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x13, (unsigned)interrupts_ISR_13, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x14, (unsigned)interrupts_ISR_14, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x15, (unsigned)interrupts_ISR_15, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x16, (unsigned)interrupts_ISR_16, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x17, (unsigned)interrupts_ISR_17, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x18, (unsigned)interrupts_ISR_18, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x19, (unsigned)interrupts_ISR_19, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x1A, (unsigned)interrupts_ISR_1A, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x1B, (unsigned)interrupts_ISR_1B, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x1C, (unsigned)interrupts_ISR_1C, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x1D, (unsigned)interrupts_ISR_1D, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x1E, (unsigned)interrupts_ISR_1E, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x1F, (unsigned)interrupts_ISR_1F, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x20, (unsigned)interrupts_ISR_20, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x21, (unsigned)interrupts_ISR_21, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x22, (unsigned)interrupts_ISR_22, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x23, (unsigned)interrupts_ISR_23, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x24, (unsigned)interrupts_ISR_24, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x25, (unsigned)interrupts_ISR_25, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x26, (unsigned)interrupts_ISR_26, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x27, (unsigned)interrupts_ISR_27, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x28, (unsigned)interrupts_ISR_28, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x29, (unsigned)interrupts_ISR_29, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x2A, (unsigned)interrupts_ISR_2A, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x2B, (unsigned)interrupts_ISR_2B, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x2C, (unsigned)interrupts_ISR_2C, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x2D, (unsigned)interrupts_ISR_2D, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x2E, (unsigned)interrupts_ISR_2E, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x2F, (unsigned)interrupts_ISR_2F, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x30, (unsigned)interrupts_ISR_30, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x31, (unsigned)interrupts_ISR_31, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x32, (unsigned)interrupts_ISR_32, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x33, (unsigned)interrupts_ISR_33, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x34, (unsigned)interrupts_ISR_34, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x35, (unsigned)interrupts_ISR_35, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x36, (unsigned)interrupts_ISR_36, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x37, (unsigned)interrupts_ISR_37, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x38, (unsigned)interrupts_ISR_38, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x39, (unsigned)interrupts_ISR_39, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x3A, (unsigned)interrupts_ISR_3A, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x3B, (unsigned)interrupts_ISR_3B, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x3C, (unsigned)interrupts_ISR_3C, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x3D, (unsigned)interrupts_ISR_3D, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x3E, (unsigned)interrupts_ISR_3E, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x3F, (unsigned)interrupts_ISR_3F, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x40, (unsigned)interrupts_ISR_40, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x41, (unsigned)interrupts_ISR_41, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x42, (unsigned)interrupts_ISR_42, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x43, (unsigned)interrupts_ISR_43, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x44, (unsigned)interrupts_ISR_44, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x45, (unsigned)interrupts_ISR_45, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x46, (unsigned)interrupts_ISR_46, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x47, (unsigned)interrupts_ISR_47, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x48, (unsigned)interrupts_ISR_48, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x49, (unsigned)interrupts_ISR_49, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x4A, (unsigned)interrupts_ISR_4A, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x4B, (unsigned)interrupts_ISR_4B, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x4C, (unsigned)interrupts_ISR_4C, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x4D, (unsigned)interrupts_ISR_4D, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x4E, (unsigned)interrupts_ISR_4E, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x4F, (unsigned)interrupts_ISR_4F, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x50, (unsigned)interrupts_ISR_50, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x51, (unsigned)interrupts_ISR_51, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x52, (unsigned)interrupts_ISR_52, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x53, (unsigned)interrupts_ISR_53, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x54, (unsigned)interrupts_ISR_54, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x55, (unsigned)interrupts_ISR_55, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x56, (unsigned)interrupts_ISR_56, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x57, (unsigned)interrupts_ISR_57, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x58, (unsigned)interrupts_ISR_58, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x59, (unsigned)interrupts_ISR_59, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x5A, (unsigned)interrupts_ISR_5A, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x5B, (unsigned)interrupts_ISR_5B, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x5C, (unsigned)interrupts_ISR_5C, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x5D, (unsigned)interrupts_ISR_5D, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x5E, (unsigned)interrupts_ISR_5E, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x5F, (unsigned)interrupts_ISR_5F, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x60, (unsigned)interrupts_ISR_60, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x61, (unsigned)interrupts_ISR_61, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x62, (unsigned)interrupts_ISR_62, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x63, (unsigned)interrupts_ISR_63, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x64, (unsigned)interrupts_ISR_64, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x65, (unsigned)interrupts_ISR_65, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x66, (unsigned)interrupts_ISR_66, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x67, (unsigned)interrupts_ISR_67, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x68, (unsigned)interrupts_ISR_68, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x69, (unsigned)interrupts_ISR_69, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x6A, (unsigned)interrupts_ISR_6A, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x6B, (unsigned)interrupts_ISR_6B, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x6C, (unsigned)interrupts_ISR_6C, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x6D, (unsigned)interrupts_ISR_6D, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x6E, (unsigned)interrupts_ISR_6E, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x6F, (unsigned)interrupts_ISR_6F, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x70, (unsigned)interrupts_ISR_70, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x71, (unsigned)interrupts_ISR_71, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x72, (unsigned)interrupts_ISR_72, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x73, (unsigned)interrupts_ISR_73, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x74, (unsigned)interrupts_ISR_74, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x75, (unsigned)interrupts_ISR_75, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x76, (unsigned)interrupts_ISR_76, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x77, (unsigned)interrupts_ISR_77, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x78, (unsigned)interrupts_ISR_78, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x79, (unsigned)interrupts_ISR_79, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x7A, (unsigned)interrupts_ISR_7A, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x7B, (unsigned)interrupts_ISR_7B, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x7C, (unsigned)interrupts_ISR_7C, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x7D, (unsigned)interrupts_ISR_7D, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x7E, (unsigned)interrupts_ISR_7E, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
	interrupts_setGate(0x7F, (unsigned)interrupts_ISR_7F, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
}

void interrupts_remap8259s(uint8 baseIRQ)
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

void interrupts_setGate(unsigned char Number, unsigned long Address, unsigned short Selector, unsigned char Flags)
{
	IDT_Table[Number].Address_Low = (Address & 0xFFFF);
	IDT_Table[Number].Address_High = (Address >> 16) & 0xFFFF;
	
	IDT_Table[Number].Selector = Selector;
	IDT_Table[Number].Empty = 0x00;
	IDT_Table[Number].Flags = Flags;
}
