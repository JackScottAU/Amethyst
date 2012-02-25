/*
 * portIO.h - Basic port handling.
 * Part of the Synergy Operating System
 * Licensed under the ISC license.
*/

#ifndef __PORTIO_H
#define __PORTIO_H

	unsigned char readByte(unsigned short int PortNumber);
	void writeByte(unsigned short int PortNumber, unsigned char Value);
	void haltCPU(void);

#endif /* __PORTIO_H */
