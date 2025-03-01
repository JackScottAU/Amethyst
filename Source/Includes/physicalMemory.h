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

extern uint32 _kernel_end;
uint32 physicalMemory_findEndOfReservedMemory(struct multiboot_moduleNode* module, uint32 count);

/**
 * Initialises the physical memory manager.
 */
void physicalMemory_initialise(struct multiboot_info* multibootData);

/**
 * Allocates a 4K block of physical RAM and returns it's physical address.
 */
void* physicalMemory_allocate();

/**
 * Marks a specific 4K block of physical memory as allocated. DO WE NEED THIS?!
 */
void physicalMemory_markAllocated(void* address);

/**
 * Marks a specific 4K block of physical memory as allocated.
 */
void physicalMemory_free(void* address);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_PHYSICALMEMORY_H_

