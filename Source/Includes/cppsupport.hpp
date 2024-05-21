/**
 *  Synergy Operating System - C++ Support
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef CPPSUPPORT_HPP__
#define CPPSUPPORT_HPP__

#include <stddef.h>
#include <memoryManager.h>
 
inline void *operator new(size_t size)
{
    return memoryManager_allocate(size);
}
 
inline void *operator new[](size_t size)
{
    return memoryManager_allocate(size);
}
 
inline void operator delete(void *p)
{
    memoryManager_free(p);
}
 
inline void operator delete[](void *p)
{
    memoryManager_free(p);
}

inline void *operator new(size_t, void *p)     throw() { return p; }
inline void *operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() { };
inline void  operator delete[](void *, void *) throw() { };

#endif
