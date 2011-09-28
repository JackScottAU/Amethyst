unsigned char ReadByte(unsigned short int PortNumber)
{
	/*
	SYNOPSIS:	Reads a byte from the specified port.
	INPUT:		Port to read from.
	OUTPUT:		Byte that was read.
	NOTES:		None.
	*/
	
	unsigned char Value;

	__asm__ __volatile__(
		"inb %w1,%b0"
		:"=a"(Value)
		:"d"(PortNumber)
	);
	
	return Value;
}

void WriteByte(unsigned short int PortNumber, unsigned char Value)
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
		:"a"(Value), "d"(PortNumber)
	);
}
