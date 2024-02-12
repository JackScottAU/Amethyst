#include <deviceTree.h>
#include <memoryManager.h>
#include <stream.h>
#include <serial.h>
#include <pciBus.h>
#include <keyboard.h>

deviceTree_Entry* deviceTree_rootEntry = NULL;

deviceTree_Entry* deviceTree_get(void) {
    return deviceTree_rootEntry;
}

void deviceTree_build(void) {
    // We should handle what happens if this code gets called twice, but we don't.

    deviceTree_rootEntry = deviceTree_createDevice("x86_32 Root Platform Device", DEVICETREE_TYPE_OTHER, NULL);
    
    deviceTree_addChild(deviceTree_rootEntry, pci_addDevicesToTree());

    if(!serial_detect(SERIAL_COM1)) {
        deviceTree_Entry* serial = deviceTree_createDevice("ISA Serial Port - COM1", DEVICETREE_TYPE_OTHER, NULL);

        deviceTree_addChild(deviceTree_rootEntry, serial);
    }

    if(!serial_detect(SERIAL_COM2)) {
        deviceTree_Entry* serial = deviceTree_createDevice("ISA Serial Port - COM2", DEVICETREE_TYPE_OTHER, NULL);

        deviceTree_addChild(deviceTree_rootEntry, serial);
    }

    deviceTree_Entry* keyboard = keyboard_initialise();
    deviceTree_addChild(deviceTree_rootEntry, keyboard);
}
