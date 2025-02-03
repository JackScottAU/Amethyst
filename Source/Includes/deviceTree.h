/**
 *  Amethyst Operating System - Definitions for kernel device driver tree.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>

#ifndef INCLUDES_DEVICETREE_H_
#define INCLUDES_DEVICETREE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32 Type; // 0 = mem, 1 = i/o, 2, irq.
    uint32 Flags; // irq number for irq. memory prefetch etc for memory. and so on.
    uint32 StartAddress; // Used for memory and Io
    uint32 Length; // Used for memory and Io
} DeviceResource;

/**
 * An entry in the kernel device driver tree. Parents are responsible for detecting and initialising their children.
*/
typedef struct deviceTree_Entry_S {
    struct deviceTree_Entry_S* next;    // pointer to next sibling of this device.
    struct deviceTree_Entry_S* child;   // pointer to first child.
    char* name;
    uint32 type;                        // determines what sort of struct data points to.
    DeviceResource* Resources; // as resources are unlikely to change much once a device is initialised, we use an array rather than a linked list.
    uint32 ResourceCount;
    void* data;
} deviceTree_Entry;

// This method is provided by the root platform device.
void deviceTree_build(void);

// This method is provided by the root platform device.
deviceTree_Entry* deviceTree_get(void);

// Prints. Provided by deviceTree.c
void deviceTree_print(void (*putChar)(char), bool detailedInfo);

deviceTree_Entry* deviceTree_createDevice(char* name, uint32 type, void* data);
void deviceTree_addSibling(deviceTree_Entry* attached, deviceTree_Entry* toAttach);
void deviceTree_addChild(deviceTree_Entry* parent, deviceTree_Entry* toAttach);

#define DEVICETREE_TYPE_UNKNOWN     0x00
#define DEVICETREE_TYPE_PCI         0x01
// ...
#define DEVICETREE_TYPE_OTHER       0xFFFFFFFF

#define DEVICE_RESOURCETYPE_MEM     0x00
#define DEVICE_RESOURCETYPE_IO      0x01
#define DEVICE_RESOURCETYPE_IRQ     0x02

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_DEVICETREE_H_
