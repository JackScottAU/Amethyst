/*
	basicIO.h - Basic port handling.
	Part of the Synergy Operating System
	
	Author:		Jack Scott (jack@jackscott.id.au).
	Last Modified:	2011-09-28.
*/

#ifndef __BASICIO_H
#define __BASICIO_H

	unsigned char ReadByte(unsigned short int PortNumber);
	void WriteByte(unsigned short int PortNumber, unsigned char Value);

#endif /* __BASICIO_H */
