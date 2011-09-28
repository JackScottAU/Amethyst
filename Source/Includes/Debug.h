/*
	Debug.h - Header for Serial driver.
	Part of the Jane Microkernel.
	
	Author:		Jack Scott (yayyak@yayyak.net).
	Last Modified:	2007-01-04.
	License:	3-clause BSD License.
	Notes:		None.
*/

#ifndef DEBUG_H
#define DEBUG_H

	#include <Registers.h>
	
	void Debug_Init();
	void Debug_Handler();
	void Debug_PutChar(char Character);
	void Debug_PutString(char *String);
	void Debug_PutHex(unsigned int Number);
	void Debug_PutVar(char *String, unsigned int Number);
	void Debug_RegDump(struct Registers_S *Registers);
	
#endif /* DEBUG_H */
