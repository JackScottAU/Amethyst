#include <deviceTree.h>
#include <memoryManager.h>
#include <stream.h>
#include <serial.h>

void deviceTree_printInternal(void (*putChar)(char), deviceTree_Entry* device, uint8 depth);
deviceTree_Entry* deviceTree_addChild(deviceTree_Entry* parent, char* name);

deviceTree_Entry* deviceTree_rootEntry = NULL;

deviceTree_Entry* deviceTree_get(void) {
    return deviceTree_rootEntry;
}

void deviceTree_build(void) {
    // We should handle what happens if this code gets called twice, but we don't.

    deviceTree_rootEntry = memoryManager_allocate(sizeof(deviceTree_Entry));
    deviceTree_rootEntry->name = "x86_32 Root Platform Device";
    deviceTree_rootEntry->next = NULL;

    deviceTree_Entry* device = deviceTree_rootEntry;
    

    if(!serial_detect(SERIAL_COM1)) {
        deviceTree_addChild(device, "ISA Serial Port - COM1");
    }

    if(!serial_detect(SERIAL_COM2)) {
        deviceTree_addChild(device, "ISA Serial Port - COM2");
    }
    
}

deviceTree_Entry* deviceTree_addChild(deviceTree_Entry* parent, char* name) {
    deviceTree_Entry* child = parent->child;

    if(child != NULL) {
        while(child->child != NULL) {
            child = child->child;
        }

        child->next = memoryManager_allocate(sizeof(deviceTree_Entry));
        child->next->next = NULL;
        child->next->child = NULL;
        child->next->name = name;

        return child->next;
    } else {
        // Add first child.
        parent->child = memoryManager_allocate(sizeof(deviceTree_Entry));
        parent->child->next = NULL;
        parent->child->child = NULL;
        parent->child->name = name;

        return child->child;
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
