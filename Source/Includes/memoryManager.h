/* 
 * File:   memoryManager.h
 * Author: Jack Scott
 *
 * Created on 30 November 2011, 11:31 AM
 */

#ifndef MEMORYMANAGER_H
#define	MEMORYMANAGER_H

#include <multiboot.h>

#ifdef	__cplusplus
extern "C" {
#endif

	//We can't use zero as an invalid pointer location in the memory manager (because it's valid). Use the end of memory instead.
	//Client code can assume zero is a bad pointer, but we can't.
	#define END_OF_MEMORY_LIST 0xFFFFFFFF
	
	typedef struct memoryManager_freeMemoryNode_s {
		uint64 address;
		uint64 length;
		struct memoryManager_freeMemoryNode_s* next;
	} memoryManager_freeMemoryNode; //Kept in order of size, small to large.
	
	typedef struct {
		uint32 size;
	} memoryManager_allocatedHeader;
	
	void memoryManager_init(struct multiboot_memoryMapNode* startAddress, uint32 length, uint32 endOfReservedMemory);
	
	void* memoryManager_allocate(uint32 size);
	void memoryManager_free(void* mem);

#ifdef	__cplusplus
}
#endif

#endif	/* MEMORYMANAGER_H */

