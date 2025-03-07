/**
 *  Amethyst Operating System - Device and driver tree core functionality.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/
#include <deviceTree.h>
#include <memoryManager.h>
#include <stream.h>
#include <Drivers/pciBus.h>

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

    // TODO: print this in bold.
    stream_printf(putChar, "\033[32m");
    stream_printf(putChar, "%s\n", device->name);
    stream_printf(putChar, "\033[0m");

    // TODO: print more info if detailedInfo is set (need to put resources and PCI class etc in here).
    if(detailedInfo) {
        // todo

        // if(type = pci) { pci_printMoreInfo(entry, depth) } to show class/subclass/if/revision and Bus/Slot/Function
        // to make this work need to put bus/slot/function into devicetree entry.

        if(device->type == DEVICETREE_TYPE_PCI) {
            pciBus_printDeviceInformation(putChar, device->data, depth);
        }

        // then interate through attached i/o, mem, irq resources, etc
        for(uint32 i = 0; i < device->ResourceCount; i++) {
            for (uint8 j = 0; j < depth; j++) {
                stream_printf(putChar, " |  ");
            }

            DeviceResource resource = device->Resources[i];

            uint32 end = resource.StartAddress + resource.Length - 1;

            switch(resource.Type)
            {
                case DEVICE_RESOURCETYPE_MEM:
                    stream_printf(putChar, "Resource (MEM): %h -> %h (%h)\n", resource.StartAddress, end, resource.Length);
                    break;

                case DEVICE_RESOURCETYPE_IO:
                    stream_printf(putChar, "Resource (I/O): %h -> %h (%h)\n", resource.StartAddress, end, resource.Length);
                    break;

                case DEVICE_RESOURCETYPE_IRQ:
                    stream_printf(putChar, "Resource (IRQ): %h\n", resource.Flags);
                    break;

                default:
                    stream_printf(putChar, "Resource (Unknown)\n");
                    break;
            }
        }
    }

    // Print children, then come back here and print the next sibling to us.
    deviceTree_printInternal(putChar, device->child, depth + 1, detailedInfo);
    deviceTree_printInternal(putChar, device->next, depth, detailedInfo);
}
