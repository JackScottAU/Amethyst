/**
 *  Amethyst Operating System - CPUID helper class.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <deviceTree.h>
#include <memoryManager.h>
#include <stream.h>
#include <serial.h>
#include <pciBus.h>
#include <keyboard.h>
#include <ps2controller.h>

deviceTree_Entry* deviceTree_rootEntry = NULL;

deviceTree_Entry* deviceTree_get(void) {
    return deviceTree_rootEntry;
}

void deviceTree_build(void) {
    // We should handle what happens if this code gets called twice, but we don't.

    deviceTree_rootEntry = deviceTree_createDevice("x86_32 Root Platform Device", DEVICETREE_TYPE_OTHER, NULL);

    deviceTree_addChild(deviceTree_rootEntry, pci_addDevicesToTree());

    if (!serial_detect(SERIAL_COM1)) {
        deviceTree_Entry* serial = deviceTree_createDevice("ISA Serial Port - COM1", DEVICETREE_TYPE_OTHER, NULL);

        deviceTree_addChild(deviceTree_rootEntry, serial);
    }

    if (!serial_detect(SERIAL_COM2)) {
        deviceTree_Entry* serial = deviceTree_createDevice("ISA Serial Port - COM2", DEVICETREE_TYPE_OTHER, NULL);

        deviceTree_addChild(deviceTree_rootEntry, serial);
    }

    // Assume a PS/2 controller exists and attempt to initialise it.
    deviceTree_Entry* ps2controller = ps2controller_initialise();
    if(ps2controller != NULL) {
        deviceTree_addChild(deviceTree_rootEntry, ps2controller);
    }
}
