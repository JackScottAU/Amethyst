/**
 *  Amethyst Operating System - Driver for ISA 16550 UART.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_SERIAL_H_
#define INCLUDES_SERIAL_H_

#include <Types.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define SERIAL_COM1         0x03F8
#define SERIAL_COM2         0x02F8

#define SERIAL_BAUD_115200  0x01
#define SERIAL_BAUD_57600   0x02
#define SERIAL_BAUD_38400   0x03
#define SERIAL_BAUD_28800   0x04
#define SERIAL_BAUD_19200   0x06
#define SERIAL_BAUD_14400   0x08
#define SERIAL_BAUD_9600    0x0C

int serial_detect(uint16 baseAddress);
int serial_init(uint16 baseAddress, uint8 divisor);
char serial_readByte(void);

int serial_canRead(void);
char serial_readChar(void);
char* serial_readString(void);

void serial_writeChar(char a);

void serial_writeString(const char* string);
void serial_writeLine(const char* string);

#ifdef	__cplusplus
}
#endif

#endif // INCLUDES_SERIAL_H_
