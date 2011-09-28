//port.h - Basic port I/O and related stuff.

//Copyright (c) 2006-2008, Jack Scott (jack@jackscott.org)
//Released under the 3-clause BSD License (see documentation/license.txt)

#ifndef PORT_H
#define PORT_H

	//TODO: Check if these work with 16b data. They were created from 8b data.
	#define readIO(port,data) __asm__ __volatile__("inb %w1,%b0":"=a"((data)):"d"((port)));
	#define writeIO(port,data) __asm__ __volatile__("outb %b0,%w1"::"a"((data)), "d"((port)));
#endif
