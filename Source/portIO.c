#include <portIO.h>
#include <Types.h>

/**
 * 
 * @param portNumber
 * @return 
 */
uint8 readByte(uint16 portNumber)
{
	/*
	SYNOPSIS:	Reads a byte from the specified port.
	INPUT:		Port to read from.
	OUTPUT:		Byte that was read.
	NOTES:		None.
	*/
	
	uint8 value;

	__asm__ __volatile__(
		"inb %w1,%b0"
		:"=a"(value)
		:"d"(portNumber)
	);
	
	return value;
}

/**
 * 
 * @param portNumber
 * @param value
 */
void writeByte(uint16 portNumber, uint8 value)
{
	/*
	SYNOPSIS:	Writes a byte to the specified port.
	INPUT:		Port to write to, value to write.
	OUTPUT:		None.
	NOTES:		None.
	*/
	
	__asm__ __volatile__(
		"outb %b0,%w1"
		:
		:"a"(value), "d"(portNumber)
	);
}
