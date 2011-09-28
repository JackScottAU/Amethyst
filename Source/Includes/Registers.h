/*

*/

#ifndef REGISTERS_H
#define REGISTERS_H

	struct Registers_S
	{
		/*
		SYNOPSIS:	Form of the registers passed to the handler.
		NOTES:		None.
		*/
		
		unsigned int GS, FS, ES, DS;
		unsigned int EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX;
		unsigned int IntNum, ErrorCode;
		unsigned int EIP, CS, EFlags, UserESP, SS;    
	};

#endif
