/**
 *  Amethyst Operating System - PC Mouse Driver.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <interrupts.h>
#include <memoryManager.h>
#include <portIO.h>
#include <Structures/fifobuffer.h>
#include <deviceTree.h>
#include <stream.h>
#include <debug.h>
#include <Drivers/ps2controller.h>
#include <Drivers/mouse.h>

typedef struct {
    sint16 x;
    sint16 y;
} MouseEvent;

MouseEvent mouseEvent;

uint8 currentMouseByte;
uint8 numberOfMouseBytes;

uint8 mouseBytes[4];

void mouse_interruptHandler(uint32 eventData);

void mouse_interruptHandler(uint32 eventData) {
    uint8 data = portIO_read8(0x60);
    debug(LOGLEVEL_DEBUG, "Mouse data received: %h", data);

    mouseBytes[currentMouseByte] = data;
    currentMouseByte++;

    if(currentMouseByte >= numberOfMouseBytes) {
        currentMouseByte = 0;
    }

    if(currentMouseByte == 0) {
        // we have a full mouse packet in the array buffer.
        if(mouseBytes[0] & 0x10) {
            mouseEvent.x = mouseBytes[1] | 0xFF00;
        }
        else {
            mouseEvent.x = mouseBytes[1];
        }

        if(mouseBytes[0] & 0x20) {
            mouseEvent.y = mouseBytes[2] | 0xFF00;
        }
        else {
            mouseEvent.y = mouseBytes[2];
        }

        debug(LOGLEVEL_INFO, "Mouse X: %d, Mouse Y: %d", mouseEvent.x, mouseEvent.y);
    }
}

deviceTree_Entry* mouse_initialise() {
    interrupts_addHandler(0x2C, 0, (*mouse_interruptHandler));

    // enable mouse data sending
    debug(LOGLEVEL_DEBUG, "PS/2 Mouse: Sending reset...");
    ps2controller_sendByteToDevice(2, 0xFF);    // RESET.
    ps2controller_receiveByteFromDevice(2);
    ps2controller_receiveByteFromDevice(2);
    ps2controller_receiveByteFromDevice(2);

    debug(LOGLEVEL_DEBUG, "PS/2 Mouse: Setting sample rate...");
    ps2controller_sendByteToDevice(2, 0xF3);    // set rate.
    ps2controller_receiveByteFromDevice(2);
    ps2controller_sendByteToDevice(2, 100);     // 100 samples per second.
    ps2controller_receiveByteFromDevice(2);

    debug(LOGLEVEL_DEBUG, "PS/2 Mouse: Enabling data sending...");
    ps2controller_sendByteToDevice(2, 0xF4);    // set rate.
    ps2controller_receiveByteFromDevice(2);

    currentMouseByte = 0;
    numberOfMouseBytes = 3;

    return deviceTree_createDevice("Generic PS/2 Mouse", DEVICETREE_TYPE_OTHER, NULL);
}
