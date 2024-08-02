/**
 *  Amethyst Operating System - Linked List code.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_STRUCTURES_LIST_H_
#define INCLUDES_STRUCTURES_LIST_H_

#include <Types.h>

typedef struct ListItem_s {
    void* data;
    struct ListItem_s* next;
} ListItem;

typedef struct {
    uint32 count;
    ListItem* firstItem;
} List;

List* List_new();
void List_add(List* list, void* data);

#endif  // INCLUDES_STRUCTURES_LIST_H_
