#include <portIO.h>

unsigned char readByte(unsigned short int portNumber)
{
	/*
	SYNOPSIS:	Reads a byte from the specified port.
	INPUT:		Port to read from.
	OUTPUT:		Byte that was read.
	NOTES:		None.
	*/
	
	unsigned char value;

	__asm__ __volatile__(
		"inb %w1,%b0"
		:"=a"(value)
		:"d"(portNumber)
	);
	
	return value;
}

void writeByte(unsigned short int portNumber, unsigned char value)
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
