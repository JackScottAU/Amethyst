/**
 *  Amethyst Operating System - Paging structures.
 *  Copyright 2026 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>


#ifndef ARCH_X86_32_PAGING_H_
#define ARCH_X86_32_PAGING_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  {
    uint8 p : 1;
    uint8 rw : 1;
    uint8 user_or_supervisor : 1;
    uint8 writeThrough : 1;
    uint8 cacheDisable : 1;
    uint8 access : 1;
    uint8 accounting1 : 1;
    uint8 pageSize : 1;
    uint8 avail : 4;
    uint32 address : 20;
} __attribute__((packed)) pageDirectoryEntry;

typedef struct  {
    uint8 p : 1;
    uint8 rw : 1;
    uint8 user_or_supervisor : 1;
    uint8 writeThrough : 1;
    uint8 cacheDisable : 1;
    uint8 access : 1;
    uint8 dirty : 1;
    uint8 pageAttributeTable : 1;
    uint8 global : 1;
    uint8 avail : 3;
    uint32 address : 20;
} __attribute__((packed)) pageTableEntry;

typedef pageDirectoryEntry* PageDirectory;


#ifdef __cplusplus
}
#endif

#endif  // ARCH_X86_32_PAGING_H_
