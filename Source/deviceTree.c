/**
 *  Amethyst Operating System - Device and driver tree core functionality.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/
#include <deviceTree.h>
#include <memoryManager.h>
#include <stream.h>

void deviceTree_printInternal(void (*putChar)(char), deviceTree_Entry* device, uint8 depth, bool detailedInfo);

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
    while (attached->next != NULL) {
        attached = attached->next;
    }

    attached->next = toAttach;
}

void deviceTree_addChild(deviceTree_Entry* parent, deviceTree_Entry* toAttach) {
    deviceTree_Entry* child = parent->child;

    if (child != NULL) {
        deviceTree_addSibling(child, toAttach);
    } else {
        // Add first child.
        parent->child = toAttach;
    }
}

void deviceTree_print(void (*putChar)(char), bool detailedInfo) {
    deviceTree_printInternal(putChar, deviceTree_get(), 0, detailedInfo);
}

void deviceTree_printInternal(void (*putChar)(char), deviceTree_Entry* device, uint8 depth, bool detailedInfo) {
    if (device == NULL) {
        return;
    }

    if (depth > 0) {
        for (int i = 0; i < depth - 1; i++) {
            stream_printf(putChar, " |  ");
        }

        stream_printf(putChar, " +- ");
    }

    stream_printf(putChar, "%s\n", device->name);

    // Print children, then come back here and print the next sibling to us.
    deviceTree_printInternal(putChar, device->child, depth + 1, detailedInfo);
    deviceTree_printInternal(putChar, device->next, depth, detailedInfo);
}
