/**
 *  Synergy Operating System - ISA 16550 UART.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_SERIAL_H_
#define INCLUDES_SERIAL_H_

#include <Types.h>

int serial_init(void);
char serial_readByte(void);
void serial_writeByte(char a);

#endif // INCLUDES_SERIAL_H_
