/*
	BasicIO.h - Basic port handling.
	Part of the Jane Microkernel.
	
	Author:		Jack Scott (yayyak@yayyak.net).
	Last Modified:	2006-11-28.
	License:	3-clause BSD License.
	Notes:		None.
*/

#ifndef BASICIO_H
#define BASICIO_H

	unsigned char ReadByte(unsigned short int PortNumber);
	void WriteByte(unsigned short int PortNumber, unsigned char Value);

#endif /* BASICIO_H */
