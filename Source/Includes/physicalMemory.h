/**
 *  Amethyst Operating System - Physical Memory Management.
 *  Copyright 2025 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_PHYSICALMEMORY_H_
#define INCLUDES_PHYSICALMEMORY_H_

#include <multiboot.h>
#include <Types.h>

#ifdef __cplusplus
extern "C" {
#endif

void physicalMemory_initialise(multiboot_memoryMapNode* multibootMemoryMap, uint32 length);

void physicalMemory_allocate();
void physicalMemory_allocate(uint32 address);

void physicalMemory_free(uint32 address);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_PHYSICALMEMORY_H_

