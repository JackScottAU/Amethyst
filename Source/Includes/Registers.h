/*

*/

#ifndef REGISTERS_H
#define REGISTERS_H

	/// @brief Form of the registers passed to the interrupt handler by the CPU.
	struct Registers_S
	{
		unsigned int GS, FS, ES, DS;
		unsigned int EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX;
		unsigned int IntNum, ErrorCode;
		unsigned int EIP, CS, EFlags, UserESP, SS;    
	};

#endif
