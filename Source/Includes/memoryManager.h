/**
 *  Amethyst Operating System - Memory management stuff (so far).
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_MEMORYMANAGER_H_
#define INCLUDES_MEMORYMANAGER_H_

#include <multiboot.h>
#include "../arch/x86_32/paging.h"

#ifdef __cplusplus
extern "C" {
#endif

// We can't use zero as an invalid pointer location in the memory manager (because it's valid).
// Use the end of memory instead.
// Client code can assume zero is a bad pointer, but we can't.
#define END_OF_MEMORY_LIST 0xFFFFFFFF

typedef struct memoryManager_freeMemoryNode_s {
    uint64 address;
    uint64 length;
    struct memoryManager_freeMemoryNode_s* next;
} memoryManager_freeMemoryNode;     // Kept in order of size, small to large.

typedef struct {
    uint32 size;
} memoryManager_allocatedHeader;

void memoryManager_init(struct multiboot_memoryMapNode* startAddress, uint32 length, uint32 endOfReservedMemory);

/**
 * Returns the address of the current page directory.
 */
PageDirectory* memoryManager_getCurrentPageDirectory();

/**
 * Maps count number of 4KiB regions of physical memory into logical memory.
 */
void memoryManager_mapPhysicalMemoryPage(PageDirectory* directory, void* startLogicalAddress, void* physicalMemory, uint32 count);

void* memoryManager_allocate(uint32 size);
void memoryManager_free(void* mem);

void memoryManager_debug_printFreeMemoryList(void);

extern uint32 _kernel_end;
uint32 memoryManager_findEndOfReservedMemory(struct multiboot_moduleNode* module, uint32 count);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_MEMORYMANAGER_H_

