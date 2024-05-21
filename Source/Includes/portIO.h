/*
 * portIO.h - Basic I/O port handling functions.
 * Part of the Synergy Operating System
 * Licensed under the ISC license.
*/

#ifndef __PORTIO_H
#define __PORTIO_H

	#include <Types.h>

	#ifdef	__cplusplus
extern "C" {
#endif

	/**
	* Reads an 8-bit value from the specified port.
	* @param portNumber Port to read from.
	* @return Byte that was read.
	*/
	uint8 portIO_read8(uint16 portNumber);

	/**
	* Writes an 8-bit value to the specified port.
	* @param portNumber Port to write to.
	* @param value The value to write.
	*/
	void portIO_write8(uint16 portNumber, uint8 Value);

	/**
	* Reads a 16-bit value from the specified port.
	* @param portNumber Port to read from.
	* @return Value that was read.
	*/
	uint16 portIO_read16(uint16 portNumber);

	/**
	* Writes a 16-bit value to the specified port.
	* @param portNumber Port to write to.
	* @param value The value to write.
	*/
	void portIO_write16(uint16 portNumber, uint16 value);

	/**
	* Reads a 32-bit value from the specified port.
	* @param portNumber Port to read from.
	* @return Value that was read.
	*/
	uint32 portIO_read32(uint16 portNumber);

	/**
	* Writes a 32-bit value to the specified port.
	* @param portNumber Port to write to.
	* @param value The value to write.
	*/
	void portIO_write32(uint16 portNumber, uint32 value);

	/**
	* Halts the CPU.
	*/
	void haltCPU(void);

	#ifdef	__cplusplus
}
#endif

#endif /* __PORTIO_H */
