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

	typedef struct memoryManager_freeMemoryNode_s {
		uint64 address;
		uint64 length;
		struct memoryManager_freeMemoryNode_s* next;
	} memoryManager_freeMemoryNode; //Kept in order of size, small to large.
	
	void memoryManager_init(struct multiboot_memoryMapNode* startAddress, uint32 length, uint32 endOfReservedMemory);


#ifdef	__cplusplus
}
#endif

#endif	/* MEMORYMANAGER_H */

