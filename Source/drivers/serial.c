/**
 *  Amethyst Operating System - Serial driver.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <interrupts.h>
#include <memoryManager.h>
#include <portIO.h>
#include <Drivers/serial.h>
#include <Structures/fifobuffer.h>

#define SERIAL_CLOCKSPEED 115200

void serial_registerHandler(void);
int serial_testLoopBack(uint16 baseAddress);

/**
 * 0 if there is a device. any other number is no device.
*/
int serial_detect(uint16 baseAddress) {
    portIO_write8(baseAddress + 7, 0xAE);

    uint8 value = portIO_read8(baseAddress + 7);

    if (value != 0xAE) {
        return 1;
    }

    if (serial_testLoopBack(baseAddress)) {
        return 1;
    }

    return 0;
}

int serial_init(uint16 baseAddress, uint8 divisor) {
    portIO_write8(baseAddress + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    portIO_write8(baseAddress + 0, divisor);    // Set divisor to 3 (lo byte) 38400 baud
    portIO_write8(baseAddress + 1, 0x00);    //                  (hi byte)
    portIO_write8(baseAddress + 3, 0x03);    // 8 bits, no parity, one stop bit (disabled DLAB)
    portIO_write8(baseAddress + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    portIO_write8(baseAddress + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    portIO_write8(baseAddress + 4, 0x0F);   // Data Terminal Ready, Request To Send, Out 1, Out 2 all enabled.
    portIO_write8(baseAddress + 1, 0x01);   // Enable data interrupts

    if (serial_testLoopBack(baseAddress)) {
        return 1;
    }

    serial_registerHandler();

    return 0;
}

/**
 * Tests the serial device loopback. Returns 0 on success, any other number is failure. Returns state to how it was before test, if test was successful.
*/
int serial_testLoopBack(uint16 baseAddress) {
    uint8 modemControlRegisterState = portIO_read8(baseAddress + 4);
    uint8 interruptControlRegisterState = portIO_read8(baseAddress + 1);

    portIO_write8(baseAddress + 4, 0x1E);    // Set in loopback mode, test the serial chip
    portIO_write8(baseAddress + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (portIO_read8(baseAddress + 0) != 0xAE) {
        return 1;
    }

    portIO_write8(baseAddress + 4, modemControlRegisterState);
    portIO_write8(baseAddress + 1, interruptControlRegisterState);

    return 0;
}

FIFOBuffer* serial_readbuffer;

bool serial_echoMode = true;

int serial_received(void);

void serial_interruptHandler(uint32 eventData);

void serial_interruptHandler(uint32 eventData) {
    uint8 data = portIO_read8(SERIAL_COM1);

    FIFOBuffer_WriteBytes(serial_readbuffer, &data, 1);

    if (serial_echoMode) {
        serial_writeChar(data);
    }
}

void serial_registerHandler() {
    interrupts_addHandler(0x24, 0, (*serial_interruptHandler));

    serial_readbuffer = FIFOBuffer_new(1024);
}

/**
 * Whether the serial read buffer has data. 0 = no data. otherwise returns size of data waiting to be read in bytes.
*/
int serial_canRead() {
    return FIFOBuffer_ContentsSize(serial_readbuffer);
}

char serial_readChar() {
    // If there's nothing in the buffer, say so.
    if (serial_canRead() == 0) {
        return 0;
    }

    uint8 data;

    data = FIFOBuffer_ReadBytes(serial_readbuffer, &data, 1);

    return (char)data;
}

/**
 * Reads the current buffer contents.
*/
char* serial_readString() {
    int stringSize = serial_canRead();
    char* string = memoryManager_allocate(sizeof(uint8) * (stringSize + 1));

    for (int i = 0; i < stringSize; i++) {
        string[i] = serial_readChar();
    }

    string[stringSize] = 0;

    return string;
}

int serial_received() {
    return portIO_read8(SERIAL_COM1 + 5) & 1;
}

char serial_readByte() {
    while (serial_received() == 0) { }

    return portIO_read8(SERIAL_COM1);
}

int is_transmit_empty(void);
int is_transmit_empty() {
    return portIO_read8(SERIAL_COM1 + 5) & 0x20;
}

void serial_writeChar(char a) {
    while (is_transmit_empty() == 0) { }

    portIO_write8(SERIAL_COM1, a);
}

void serial_writeString(const char* string) {
    int i = 0;

    while (string[i]) {
        serial_writeChar(string[i]);
        i++;
    }
}

void serial_writeLine(const char* string) {
    serial_writeString(string);

    serial_writeChar('\r');
    serial_writeChar('\n');
}
