#include <stddef.h>
#include <memoryManager.h>
#include <cppsupport.hpp>

void *operator new(size_t size)
{
    return memoryManager_allocate(size);
}
 
void *operator new[](size_t size)
{
    return memoryManager_allocate(size);
}
 
void operator delete(void *p)
{
    //free(p);
}
 
void operator delete[](void *p)
{
    //free(p);
}