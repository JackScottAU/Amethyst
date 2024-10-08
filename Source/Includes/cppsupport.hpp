/**
 *  Amethyst Operating System - C++ Support
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
 * 
 *  This file is automatically included in compilation for all C++ files by the Makefile/GCC.
*/

#ifndef INCLUDES_CPPSUPPORT_HPP_
#define INCLUDES_CPPSUPPORT_HPP_

#include <stddef.h>
#include <memoryManager.h>

inline void *operator new(size_t size) {
    return memoryManager_allocate(size);
}

inline void *operator new[](size_t size) {
    return memoryManager_allocate(size);
}

inline void operator delete(void *p) {
    memoryManager_free(p);
}

inline void operator delete[](void *p) {
    memoryManager_free(p);
}

inline void *operator new(size_t, void *p)     throw() { return p; }
inline void *operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, uint32) throw() { }
inline void  operator delete[](void *, uint32) throw() { }

#endif  // INCLUDES_CPPSUPPORT_HPP_
