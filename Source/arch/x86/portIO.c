#include <portIO.h>
#include <Types.h>

/**
 * Reads a byte from the specified port.
 * @param portNumber Port to read from.
 * @return Byte that was read.
 */
uint8 readByte(uint16 portNumber)
{
	uint8 value;

	__asm__ __volatile__(
		"inb %w1,%b0"
		:"=a"(value)
		:"d"(portNumber)
	);
	
	return value;
}

/**
 * Writes a byte to the specified port.
 * @param portNumber Port to write to.
 * @param value The value to write.
 */
void writeByte(uint16 portNumber, uint8 value)
{
	__asm__ __volatile__(
		"outb %b0,%w1"
		:
		:"a"(value), "d"(portNumber)
	);
}

uint32 readLong(uint16 portNumber)
{
	uint32 value;

	__asm__ __volatile__(
		"inl %w1,%0"
		:"=a"(value)
		:"d"(portNumber)
	);
	
	return value;
}

void writeLong(uint16 portNumber, uint32 value)
{
	__asm__ __volatile__(
		"outl %0,%w1"
		:
		:"a"(value), "d"(portNumber)
	);
}

void haltCPU(void)
{
	__asm__ __volatile__("hlt");
}