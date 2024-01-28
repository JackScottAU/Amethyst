#include <Types.h>

#ifndef INCLUDES_DEICETREE_H_
#define INCLUDES_DEICETREE_H_

typedef struct deviceTree_Entry_S {
    char* name;
    struct deviceTree_Entry_S* next;
    struct deviceTree_Entry_S* child;
    void* data;
} deviceTree_Entry;

void deviceTree_build(void);
deviceTree_Entry* deviceTree_get(void);
void deviceTree_print(void (*putChar)(char));

#endif // INCLUDES_DEICETREE_H_
