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


void mouse_interruptHandler(uint32 eventData);

void mouse_interruptHandler(uint32 eventData) {
    uint8 data = portIO_read8(0x60);

    debug(LOGLEVEL_DEBUG, "Mouse data received: %h", data);
}

deviceTree_Entry* mouse_initialise() {
    interrupts_addHandler(0x2C, 0, (*mouse_interruptHandler));


    return deviceTree_createDevice("Generic PS/2 Mouse", DEVICETREE_TYPE_OTHER, NULL);
}