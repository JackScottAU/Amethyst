/*
	Debug.c - Serial Debugging Console Driver.
	Part of the Jane Microkernel.
	
	Maintainer:	Jack Scott (yayyak@yayyak.net).
	Last Modified:	2007-01-17.
	License:	3-clause BSD License.
	Notes:		This code implements a TTY terminal over the COM1 serial
			port to provide a means of debugging the kernel. This
			module should rely on as few other modules as possible,
			and to a great extent this has been acheived. For
			additional notes see the file 'Debugging.txt' in the
			documentation directory.
*/

#include <BasicIO.h>	//ReadByte, WriteByte.
#include <Clock.h>	//Clock_Uptime().
#include <Config.h>	//DEBUG, VERSION_STRING.
#include <Debug.h>
#include <Registers.h>	//Registers_S.

//Don't bother compiling this in if we don't need DEBUG support. It's just a 
//waste of a huge amount of disk space and memory.
#ifdef DEBUG

void Debug_Init()
{
	/*
	SYNOPSIS:	Initialises COM1, then prints a few welcoming messages.
	INPUTS:		None.
	OUTPUT:		None.
	NOTES:		All the settings to the UART are hardcoded. 38400 baud
			is used in the format 8/N/1. IRQ generation is enabled
			for the time when we want to implement the ever-elusive
			debugging command set.
	*/
	
	//Write information to the UART chip.
	WriteByte(0x3F8+1, 0x00);
	WriteByte(0x3F8+3, 0x80);
	WriteByte(0x3F8+0, 0x03);
	WriteByte(0x3F8+1, 0x00);
	WriteByte(0x3F8+3, 0x03);
	WriteByte(0x3F8+2, 0xC7);
	WriteByte(0x3F8+4, 0x0B);
	WriteByte(0x3F8+1, 0x01);
	
	//Write a few welcoming messages to the debugging console.
	Debug_PutString("*** Jane Debugging Console ***\n\n");
	Debug_PutString("Kernel Version: " VERSION_STRING "\n");
}

void Debug_Handler(struct Registers_S *Registers)
{
	/*
	SYNOPSIS:	Handles a request from the debugging console.
	INPUTS:		Registers to print in case of 'r'.
	OUTPUT:		None.
	NOTES:		See Doc/Debugging.txt for more information on possible
			commands.
	*/
	
	unsigned int *MemAddr;
	
	//Keep checking registers until a character can safely be read.
	while((ReadByte(0x3F8+5)&0x01) == 0 )
		continue;
	
	//Switch through all the possible commands.
	switch(ReadByte(0x3F8))
	{
	case '?':
		Debug_PutString("**POSSIBLE COMMANDS:**\n");
		Debug_PutString("? - Print this help.\n");
		Debug_PutString("c - Print clock information.\n");
		Debug_PutString("k - Kill the system.\n");
		Debug_PutString("m - Print Memory Dump (0MiB->2MiB).\n");
		Debug_PutString("r - Print Register Dump.\n");
		break;
	
	case 'c':
		Debug_PutVar("Uptime (secs)",Clock_Uptime());
		break;
	
	case 'k':
		Debug_PutString("Killing Kernel...\n");
		__asm__ volatile("cli");
		__asm__ volatile("hlt");
		break;
	
	case 'm':
		Debug_PutString("**MEMORY DUMP 0MiB->2MiB**\n");
		MemAddr = 0x00000000;
		while((unsigned int)MemAddr<=0x00200000)
		{
			Debug_PutHex((unsigned int) MemAddr);
			Debug_PutString(":    ");
			Debug_PutHex((unsigned int) *MemAddr);
			Debug_PutString("   ");
			Debug_PutHex((unsigned int) *(MemAddr+1));
			Debug_PutString("   ");
			Debug_PutHex((unsigned int) *(MemAddr+2));
			Debug_PutString("   ");
			Debug_PutHex((unsigned int) *(MemAddr+3));
			Debug_PutString("\n");
			MemAddr = MemAddr + 4;
		}
		Debug_PutString("**END MEMORY DUMP**\n");
	
	case 'r':
		Debug_PutString("**USER REQUESTED REGISTER DUMP**\n");
		Registers->IntNum = 0;	//So as not to confuse the user.
		Debug_RegDump(Registers);
		Debug_PutString("**END REGISTER DUMP**\n");
		break;
	
	default:
		Debug_PutString("Unsupported Request.\n");
		break;
	}
}

void Debug_PutChar(char Character)
{
	/*
	SYNOPSIS:	Writes a single character to the debugging console.
	INPUTS:		The character to be written.
	OUTPUT:		None.
	NOTES:		This function does newline translation on the character
			to be written, to conform with TTY. Because of this,
			please ONLY write \n to this function.
	*/
	
	//Keep checking the buffer until a character can be written.
	while((ReadByte(0x3F8+5)&0x20) == 0 )
		continue;
	
	//Handle the newline correctly.
	if(Character=='\n')
	{
		WriteByte(0x3F8, '\n');
		WriteByte(0x3F8, '\r');
	} else {
		WriteByte(0x3F8, Character);
	}
}

void Debug_PutHex(unsigned int Number)
{
	/*
	SYNOPSIS:	Writes a number to the debugging console in hex format.
	INPUTS:		The number to write.
	OUTPUT:		None.
	NOTES:		The format of the number is 0xFFFFFFFF, where F can be
			any hex digit. No other processing is done, and no
			newline is added.
	*/
	
	unsigned int Temp = 0;
	unsigned int Power = 28;
	
	Debug_PutChar('0');
	Debug_PutChar('x');
	
	//This was a for loop... but it didn't work properly...
	while((Power+4))
	{
		Temp = (Number & (0xF << Power)) >> Power;
		//At this point, Temp cannot have a value greater than 15.
		
		if(Temp < 10)
		{
			Debug_PutChar('0'+Temp);
		} else {
			Debug_PutChar('A'+(Temp-10));
		};
		
		Power=Power-4;
	}
}

void Debug_PutString(char *String)
{
	/*
	SYNOPSIS:	Writes a string to the debugging console.
	INPUTS:		C-style string with \n newline encoding.
	OUTPUT:		None.
	NOTES:		This function relies on Debug_WriteChar() to do all the
			grunt work. It only writes pure strings, no printf-like
			escape sequences are supported.
	*/
	
	while(*String)
	{
		Debug_PutChar(*String);
		String++;
	}
}

void Debug_PutVar(char *String, unsigned int Number)
{
	/*
	SYNOPSIS:	Standard method of outputting variable name and value.
	INPUTS:		Name of variable, variable's value.
	OUTPUT:		None.
	NOTES:		Recommended Usage:
				Debug_PutVar("MB_Magic",MB_Magic);
	*/
	
	Debug_PutString(String);
	Debug_PutString(": ");
	Debug_PutHex(Number);
	Debug_PutString("\n");
}

void Debug_RegDump(struct Registers_S *Registers)
{
	Debug_PutVar("Interrupt#",Registers->IntNum);
	Debug_PutVar("Error Code",Registers->ErrorCode);
	
	Debug_PutString("EIP: ");
	Debug_PutHex(Registers->EIP);
	Debug_PutString("   ");
	Debug_PutString("CS:  ");
	Debug_PutHex(Registers->CS);
	Debug_PutString("   ");
	Debug_PutString("SS:  ");
	Debug_PutHex(Registers->SS);
	Debug_PutString("   ");
	Debug_PutString("EFlags: ");
	Debug_PutHex(Registers->EFlags);
	Debug_PutString("\n");
	
	Debug_PutString("EAX: ");
	Debug_PutHex(Registers->EAX);
	Debug_PutString("   ");
	Debug_PutString("EBX: ");
	Debug_PutHex(Registers->EBX);
	Debug_PutString("   ");
	Debug_PutString("ECX: ");
	Debug_PutHex(Registers->ECX);
	Debug_PutString("   ");
	Debug_PutString("EDX:    ");
	Debug_PutHex(Registers->EDX);
	Debug_PutString("\n");
	
	Debug_PutString("ESP: ");
	Debug_PutHex(Registers->ESP);
	Debug_PutString("   ");
	Debug_PutString("EBP: ");
	Debug_PutHex(Registers->EBP);
	Debug_PutString("   ");
	Debug_PutString("EDI: ");
	Debug_PutHex(Registers->EDI);
	Debug_PutString("   ");
	Debug_PutString("ESI:    ");
	Debug_PutHex(Registers->ESI);
	Debug_PutString("\n");
	
	Debug_PutString("DS:  ");
	Debug_PutHex(Registers->DS);
	Debug_PutString("   ");
	Debug_PutString("ES:  ");
	Debug_PutHex(Registers->ES);
	Debug_PutString("   ");
	Debug_PutString("FS:  ");
	Debug_PutHex(Registers->FS);
	Debug_PutString("   ");
	Debug_PutString("GS:     ");
	Debug_PutHex(Registers->GS);
	Debug_PutString("\n");
}

#endif
