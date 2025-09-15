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
#include <Graphics/canvas.h>
#include <Drivers/qemuVga.h>

typedef struct {
    sint16 x;
    sint16 y;
} MouseEvent;

MouseEvent mouseEvent;

uint8 currentMouseByte;
uint8 numberOfMouseBytes;

uint8 mouseBytes[4];

uint16 mouse_maxX;
uint16 mouse_maxY;

uint16 mouse_currentX;
uint16 mouse_currentY;

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

        mouse_currentX += mouseEvent.x;
        if(mouse_currentX >= mouse_maxX) {
            mouse_currentX = mouse_maxX -1;
        }
        
        mouse_currentY += mouseEvent.y * -1;
        if(mouse_currentY >= mouse_maxY) {
            mouse_currentY = mouse_maxY -1;
        }

        Canvas* canvas = qemuVga_getCanvas();

        canvas_drawRect(canvas, mouse_currentX, mouse_currentY, 1, 8, 0xFFFFFFFF);
        canvas_drawRect(canvas, mouse_currentX, mouse_currentY, 8, 1, 0xFFFFFFFF);
    }
}

uint16 mouse_getX() {
    return mouse_currentX;
}


uint16 mouse_getY() {
    return mouse_currentY;
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
    ps2controller_sendByteToDevice(2, 300);     // 100 samples per second.
    ps2controller_receiveByteFromDevice(2);

    debug(LOGLEVEL_DEBUG, "PS/2 Mouse: Enabling data sending...");
    ps2controller_sendByteToDevice(2, 0xF4);    // set rate.
    ps2controller_receiveByteFromDevice(2);

    currentMouseByte = 0;
    numberOfMouseBytes = 3;

    mouse_maxX = 1024;
    mouse_maxY = 768;

    mouse_currentX = mouse_maxX / 2;
    mouse_currentY = mouse_maxY / 2;

    return deviceTree_createDevice("Generic PS/2 Mouse", DEVICETREE_TYPE_OTHER, NULL);
}
