/*
 * portIO.h - Basic port handling.
 * Part of the Synergy Operating System
 * Licensed under the ISC license.
*/

#ifndef __PORTIO_H
#define __PORTIO_H

	#include <Types.h>

	unsigned char readByte(unsigned short int PortNumber);
	void writeByte(unsigned short int PortNumber, unsigned char Value);
	uint32 readLong(uint16 portNumber);
	void writeLong(uint16 portNumber, uint32 value);
	void haltCPU(void);

#endif /* __PORTIO_H */
