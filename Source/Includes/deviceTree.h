#include <Types.h>

#ifndef INCLUDES_DEICETREE_H_
#define INCLUDES_DEICETREE_H_

typedef struct deviceTree_Entry_S {
    struct deviceTree_Entry_S* next;
    struct deviceTree_Entry_S* child;
    char* name;
    uint32 type; // determines what sort of struct data points to.
    void* data;
} deviceTree_Entry;

void deviceTree_build(void);
deviceTree_Entry* deviceTree_get(void);
void deviceTree_print(void (*putChar)(char));

deviceTree_Entry* deviceTree_createDevice(char* name, uint32 type, void* data);
void deviceTree_addSibling(deviceTree_Entry* attached, deviceTree_Entry* toAttach);
void deviceTree_addChild(deviceTree_Entry* parent, deviceTree_Entry* toAttach);

#define DEVICETREE_TYPE_UNKNOWN     0x00
#define DEVICETREE_TYPE_PCI         0x01
// ...
#define DEVICETREE_TYPE_OTHER       0xFFFFFFFF

#endif // INCLUDES_DEICETREE_H_
