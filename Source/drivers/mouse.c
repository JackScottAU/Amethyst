/**
 *  Amethyst Operating System - PC Mouse Driver.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <keyboard.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <portIO.h>
#include <vgaConsole.h>
#include <serial.h>
#include <Structures/fifobuffer.h>
#include <deviceTree.h>
#include <stream.h>
#include <debug.h>
#include <ps2controller.h>


void mouse_interruptHandler(uint32 eventData);

void mouse_interruptHandler(uint32 eventData) {
    uint8 data = portIO_read8(0x60);

    debug(LOGLEVEL_DEBUG, "Mouse data received: %h", data);
}

deviceTree_Entry* mouse_initialise() {
    interrupts_addHandler(0x2C, 0, (*mouse_interruptHandler));

    // enable mouse data sending
    stream_printf(serial_writeChar, "sending mouse reset...\n");

    ps2controller_sendByteToDevice(2, 0xFF); // RESET.
    ps2controller_receiveByteFromDevice(2);
    ps2controller_receiveByteFromDevice(2);
    ps2controller_receiveByteFromDevice(2);
    
    stream_printf(serial_writeChar, "sending mouse rate...\n");
    ps2controller_sendByteToDevice(2, 0xF3); // set rate.
    ps2controller_receiveByteFromDevice(2);
    ps2controller_sendByteToDevice(2, 100); // 100 samples per second.
    ps2controller_receiveByteFromDevice(2);

    stream_printf(serial_writeChar, "sending mouse enable data sending...\n");
    ps2controller_sendByteToDevice(2, 0xF4); // set rate.
    ps2controller_receiveByteFromDevice(2);

    return deviceTree_createDevice("Generic PS/2 Mouse", DEVICETREE_TYPE_OTHER, NULL);
}