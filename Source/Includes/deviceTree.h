/**
 *  Synergy Operating System - Definitions for kernel device driver tree.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef INCLUDES_DEVICETREE_H_
#define INCLUDES_DEVICETREE_H_

/**
 * An entry in the kernel device driver tree. Parents are responsible for detecting and initialising their children.
*/
typedef struct deviceTree_Entry_S {
    struct deviceTree_Entry_S* next; // pointer to next sibling of this device.
    struct deviceTree_Entry_S* child; // pointer to first child.
    char* name;
    uint32 type; // determines what sort of struct data points to.
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

#endif // INCLUDES_DEVICETREE_H_

#ifdef	__cplusplus
}
#endif
