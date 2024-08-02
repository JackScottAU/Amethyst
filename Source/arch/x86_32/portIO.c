/**
 *  Amethyst Operating System - Port I/O.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <portIO.h>
#include <Types.h>

uint8 portIO_read8(uint16 portNumber) {
    uint8 value;

    __asm__ __volatile__(
        "inb %w1,%b0"
        :"=a"(value)
        :"d"(portNumber)
    );

    return value;
}

void portIO_write8(uint16 portNumber, uint8 value) {
    __asm__ __volatile__(
        "outb %b0,%w1"
        :
        :"a"(value), "d"(portNumber)
    );
}

uint16 portIO_read16(uint16 portNumber) {
    uint16 value;

    __asm__ __volatile__(
        "inw %w1,%w0"
        :"=a"(value)
        : "d"(portNumber)
    );

    return value;
}

void portIO_write16(uint16 portNumber, uint16 value) {
    __asm__ __volatile__(
        "outw %w0,%w1"
        :
        : "a"(value), "d"(portNumber)
    );
}

uint32 portIO_read32(uint16 portNumber) {
    uint32 value;

    __asm__ __volatile__(
        "inl %w1,%0"
        :"=a"(value)
        :"d"(portNumber)
    );

    return value;
}

void portIO_write32(uint16 portNumber, uint32 value) {
    __asm__ __volatile__(
        "outl %0,%w1"
        :
        :"a"(value), "d"(portNumber)
    );
}

void haltCPU(void) {
    __asm__ __volatile__("hlt");
}
