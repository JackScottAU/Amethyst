#include <stddef.h>
#include <memoryManager.h>
 
void *operator new(size_t size);
 
void *operator new[](size_t size);
 
void operator delete(void *p);
 
void operator delete[](void *p);

inline void *operator new(size_t, void *p)     throw() { return p; }
inline void *operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() { };
inline void  operator delete[](void *, void *) throw() { };
