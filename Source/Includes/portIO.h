/*
	basicIO.h - Basic port handling.
	Part of the Synergy Operating System
	
	Author:		Jack Scott (jack@jackscott.id.au).
	Last Modified:	2011-09-28.
*/

#ifndef __PORTIO_H
#define __PORTIO_H

	unsigned char readByte(unsigned short int PortNumber);
	void writeByte(unsigned short int PortNumber, unsigned char Value);

#endif /* __PORTIO_H */
