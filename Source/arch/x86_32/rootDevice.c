/**
 *  Amethyst Operating System - CPUID helper class.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <deviceTree.h>
#include <memoryManager.h>
#include <stream.h>
#include <portIO.h>
#include <debug.h>
#include <Drivers/serial.h>
#include <Drivers/pciBus.h>
#include <Drivers/ps2controller.h>

deviceTree_Entry* deviceTree_rootEntry = NULL;

deviceTree_Entry* deviceTree_get(void) {
    return deviceTree_rootEntry;
}

void deviceTree_build(void) {
    // We should handle what happens if this code gets called twice, but we don't.

    deviceTree_rootEntry = deviceTree_createDevice("x86_32 Root Platform Device", DEVICETREE_TYPE_OTHER, NULL);

    deviceTree_addChild(deviceTree_rootEntry, pciBus_initialise());

    if (!serial_detect(SERIAL_COM1)) {
        deviceTree_Entry* serial = deviceTree_createDevice("ISA Serial Port - COM1", DEVICETREE_TYPE_OTHER, NULL);

        serial->Resources = memoryManager_allocate(sizeof(DeviceResource) * 2);
        serial->ResourceCount = 2;

        serial->Resources[0].Type = DEVICE_RESOURCETYPE_IRQ;
        serial->Resources[0].Flags = 4;
        
        serial->Resources[1].Type = DEVICE_RESOURCETYPE_IO;
        serial->Resources[1].StartAddress = 0x3F8;
        serial->Resources[1].Length = 8;

        deviceTree_addChild(deviceTree_rootEntry, serial);
    }

    if (!serial_detect(SERIAL_COM2)) {
        deviceTree_Entry* serial = deviceTree_createDevice("ISA Serial Port - COM2", DEVICETREE_TYPE_OTHER, NULL);

        serial->Resources = memoryManager_allocate(sizeof(DeviceResource) * 2);
        serial->ResourceCount = 2;

        serial->Resources[0].Type = DEVICE_RESOURCETYPE_IRQ;
        serial->Resources[0].Flags = 3;
        
        serial->Resources[1].Type = DEVICE_RESOURCETYPE_IO;
        serial->Resources[1].StartAddress = 0x2F8;
        serial->Resources[1].Length = 8;

        deviceTree_addChild(deviceTree_rootEntry, serial);
    }

    // Assume a PS/2 controller exists and attempt to initialise it.
    deviceTree_Entry* ps2controller = ps2controller_initialise();
    if(ps2controller != NULL) {
        deviceTree_addChild(deviceTree_rootEntry, ps2controller);
    }

    // See if we have a parallel port.
    uint16* parallel1 = (uint16*) 0xC0000408;
    debug(LOGLEVEL_DEBUG, "PARALLEL: %h", *parallel1);
    uint16* parallel2 = (uint16*) 0xC000040A;
    debug(LOGLEVEL_DEBUG, "PARALLEL: %h", *parallel2);
    uint16* parallel3 = (uint16*) 0xC000040C;
    debug(LOGLEVEL_DEBUG, "PARALLEL: %h", *parallel3);

    if(*parallel1 > 0) {
        deviceTree_Entry* parallel = deviceTree_createDevice("Parallel Port - LPT1", DEVICETREE_TYPE_OTHER, NULL);
        parallel->Resources = memoryManager_allocate(sizeof(DeviceResource) * 2);
        parallel->ResourceCount = 2;

        
        parallel->Resources[0].Type = DEVICE_RESOURCETYPE_IRQ;
        parallel->Resources[0].Flags = 7;
        
        parallel->Resources[1].Type = DEVICE_RESOURCETYPE_IO;
        parallel->Resources[1].StartAddress = (*parallel1);
        parallel->Resources[1].Length = 8;

        deviceTree_addChild(deviceTree_rootEntry, parallel);
    }

    if(*parallel2 > 0) {
        deviceTree_Entry* parallel = deviceTree_createDevice("Parallel Port - LPT2", DEVICETREE_TYPE_OTHER, NULL);
        parallel->Resources = memoryManager_allocate(sizeof(DeviceResource) * 2);
        parallel->ResourceCount = 2;

        
        parallel->Resources[0].Type = DEVICE_RESOURCETYPE_IRQ;
        parallel->Resources[0].Flags = 7;
        
        parallel->Resources[1].Type = DEVICE_RESOURCETYPE_IO;
        parallel->Resources[1].StartAddress = (*parallel2);
        parallel->Resources[1].Length = 8;

        deviceTree_addChild(deviceTree_rootEntry, parallel);
    }

    if(*parallel3 > 0) {
        deviceTree_Entry* parallel = deviceTree_createDevice("Parallel Port - LPT3", DEVICETREE_TYPE_OTHER, NULL);
        parallel->Resources = memoryManager_allocate(sizeof(DeviceResource) * 2);
        parallel->ResourceCount = 2;

        
        parallel->Resources[0].Type = DEVICE_RESOURCETYPE_IRQ;
        parallel->Resources[0].Flags = 7;
        
        parallel->Resources[1].Type = DEVICE_RESOURCETYPE_IO;
        parallel->Resources[1].StartAddress = (*parallel3);
        parallel->Resources[1].Length = 8;

        deviceTree_addChild(deviceTree_rootEntry, parallel);
    }

  /*  deviceTree_addChild(deviceTree_rootEntry, deviceTree_createDevice("Real Time Clock?! - have this, need to put in tree", DEVICETREE_TYPE_OTHER, NULL));
    deviceTree_addChild(deviceTree_rootEntry, deviceTree_createDevice("PIT?!", DEVICETREE_TYPE_OTHER, NULL));
    deviceTree_addChild(deviceTree_rootEntry, deviceTree_createDevice("8259s? pic", DEVICETREE_TYPE_OTHER, NULL));
    deviceTree_addChild(deviceTree_rootEntry, deviceTree_createDevice("pc speaker?! - hangs off pit", DEVICETREE_TYPE_OTHER, NULL));
    deviceTree_addChild(deviceTree_rootEntry, deviceTree_createDevice("parallel prt? - i think this is a 8255 and a few latch chips in disguise", DEVICETREE_TYPE_OTHER, NULL));
    deviceTree_addChild(deviceTree_rootEntry, deviceTree_createDevice("8257 dma? x2", DEVICETREE_TYPE_OTHER, NULL));
    deviceTree_addChild(deviceTree_rootEntry, deviceTree_createDevice("fdc controller and empty drive", DEVICETREE_TYPE_OTHER, NULL));

    deviceTree_addChild(deviceTree_rootEntry, deviceTree_createDevice("pvpanic / qemu device", DEVICETREE_TYPE_OTHER, NULL));
    deviceTree_addChild(deviceTree_rootEntry, deviceTree_createDevice("hpet", DEVICETREE_TYPE_OTHER, NULL)); // replacement for pit, along with lapic timer
    deviceTree_addChild(deviceTree_rootEntry, deviceTree_createDevice("ioapic", DEVICETREE_TYPE_OTHER, NULL));
    deviceTree_addChild(deviceTree_rootEntry, deviceTree_createDevice("piix4 i2c", DEVICETREE_TYPE_OTHER, NULL));*/
}
