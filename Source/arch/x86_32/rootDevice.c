#include <deviceTree.h>
#include <memoryManager.h>
#include <stream.h>
#include <serial.h>

void deviceTree_printInternal(void (*putChar)(char), deviceTree_Entry* device, uint8 depth);

deviceTree_Entry* deviceTree_rootEntry = NULL;

deviceTree_Entry* deviceTree_get(void) {
    return deviceTree_rootEntry;
}

void deviceTree_build(void) {
    // We should handle what happens if this code gets called twice, but we don't.

    deviceTree_rootEntry = memoryManager_allocate(sizeof(deviceTree_Entry));
    deviceTree_rootEntry->name = "x86_32 Root Platform Device";
    deviceTree_rootEntry->next = NULL;
    deviceTree_rootEntry->child = NULL;

    deviceTree_Entry* device = deviceTree_rootEntry;

    deviceTree_Entry* pcidev = pci_addDevicesToTree();

    device->child = pcidev;
    

    if(!serial_detect(SERIAL_COM1)) {
        deviceTree_Entry* serial = deviceTree_createDevice("ISA Serial Port - COM1", DEVICETREE_TYPE_OTHER, NULL);

        deviceTree_addChild(deviceTree_rootEntry, serial);
    }

    if(!serial_detect(SERIAL_COM2)) {
        deviceTree_Entry* serial = deviceTree_createDevice("ISA Serial Port - COM2", DEVICETREE_TYPE_OTHER, NULL);

        deviceTree_addChild(deviceTree_rootEntry, serial);
    }

    
    

    // 0:0:0 is the PCI root hub (northbridge)
    // Everything else is below that.
}

deviceTree_Entry* deviceTree_createDevice(char* name, uint32 type, void* data) {
    deviceTree_Entry* device = memoryManager_allocate(sizeof(deviceTree_Entry));

    device->next = NULL;
    device->child = NULL;
    device->name = name;
    device->type = type;
    device->data = data;

    return device;
}

void deviceTree_addSibling(deviceTree_Entry* attached, deviceTree_Entry* toAttach) {
    while(attached->next != NULL) {
        attached = attached->next;
    }

    attached->next = toAttach;
}

void deviceTree_addChild(deviceTree_Entry* parent, deviceTree_Entry* toAttach) {
    deviceTree_Entry* child = parent->child;

    if(child != NULL) {
        deviceTree_addSibling(child, toAttach);
    } else {
        // Add first child.
        parent->child = toAttach;
    }
}

void deviceTree_print(void (*putChar)(char)) {
    deviceTree_printInternal(putChar, deviceTree_rootEntry, 0);
 /*   stream_printf(putChar, deviceTree_rootEntry->name);
    stream_printf(putChar, "\n");

    deviceTree_Entry* device = deviceTree_rootEntry->child;

    while(device != NULL) {
        deviceTree_printInternal(putChar, device, 0);
        device = device->next;
    }*/
}

void deviceTree_printInternal(void (*putChar)(char), deviceTree_Entry* device, uint8 depth) {
    if(device == NULL) {
        return;
    }

    if(depth > 0) {
        for(int i = 0; i < depth - 1; i++) {
            stream_printf(putChar, " |  ");
        }

        stream_printf(putChar, " +- ");
    }

    

    stream_printf(putChar, "%s\n", device->name);

    deviceTree_printInternal(putChar, device->child, depth + 1);
    deviceTree_printInternal(putChar, device->next, depth);



   // stream_printf(putChar, device->name);
   // stream_printf(putChar, "\n");
}
