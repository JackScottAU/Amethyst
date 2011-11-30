/**
 * memoryManager.c - Memory management functions.
 * Part of the Synergy operating system.
 * Licensed under the ISC license.
 * 
 * This file contains functions to allocate and free a block of memory (to be 
 * used by the interpreter and other kernel functions), as well as support code 
 * for initialising and mainting the list of free memory regions.
 */

#include <memoryManager.h>
#include <multiboot.h>
#include <vgaConsole.h>		//This is only used for debugging.


//Holds the address of the start of the list representing free memory blocks.
//When the kernel is first started, this list is empty.
memoryManager_freeMemoryNode* memoryManager_firstFreeNode = (memoryManager_freeMemoryNode*) END_OF_MEMORY_LIST;


/**
 * Allocates a block of memory of the correct size. Currently uses a first-fit algorithm.
 * @param size The size of the requested block of memory.
 * @return A pointer to the block of memory.
 */
void* memoryManager_allocate(uint32 size)
{
	//TODO: Switch from a first-fit to a best-fit algorithm.
	
	//A pointer to the header that is before the allocated block.
	//We allocate space for this header once we have found a free space for it.
	memoryManager_allocatedHeader* memoryHeader;
	
	//A pointer to the list of free memory spaces.
	memoryManager_freeMemoryNode* current = memoryManager_firstFreeNode;
	
	//Once we have traversed the list, this pointer holds the best free memory space.
	memoryManager_freeMemoryNode* chosen = (memoryManager_freeMemoryNode*) END_OF_MEMORY_LIST;
	
	//Traverse the list.
	while((uint32) current != END_OF_MEMORY_LIST)
	{
		if(current->length > (size+sizeof(memoryManager_allocatedHeader)))
		{
			//We've found a good space. Use it.
			chosen = current;
			break;
		}
			
		current = current->next;
	}
	
	//We now have a free block of memory. chosen->address points to the start of this area.
	
	//Create the allocated memory header at the start of the free area.
	memoryHeader = (memoryManager_allocatedHeader*) (uint32) chosen->address;
	
	//In the header we store how much was allocated. Maybe we'll store other things later?
	memoryHeader->size = size;
	
	//Update the address and length of the free area accordingly.
	chosen->address = chosen->address + size + sizeof(memoryManager_allocatedHeader);
	chosen->length = chosen->length - size - sizeof(memoryManager_allocatedHeader);
	
	//Return the first usable byte of the allocated block.
	return (void*) ((uint32)memoryHeader + (uint32)sizeof(memoryManager_allocatedHeader));
}

/**
 * Frees a block of memory that was allocated using memoryManager_allocate().
 * @param mem The first usable byte (after the header) of the allocated memory block.
 */
void memoryManager_free(void* mem)
{
	//Do nothing.
	if(mem){};
}

/**
 * Uses the map of memory supplied by the multiboot loader to create a list of free memory.
 * @param memNode An array of all the memory map records passed from the multiboot loader.
 * @param length The length in bytes of the memory map array.
 * @param endOfReservedMemory A pointer to the end of the kernel (or module's) code, so we don't overwrite it.
 */
void memoryManager_init(struct multiboot_memoryMapNode* memNode, uint32 length, uint32 endOfReservedMemory)
{
	//Loop through each of the memory map records. Each record is 24 bytes long.
	for(uint32 i=0; i<length/24; i++)
	{
		//We need to check if it is available RAM (type 1) and that the size is worth recording.
		if(memNode[i].type==1 && memNode[i].len>sizeof(memoryManager_freeMemoryNode))
		{
			//We need to be able to address the free memory record somehow.
			memoryManager_freeMemoryNode* node;
				
			//Check if we're overwriting kernels or modules.
			if( (uint32)memNode[i].addr >= 0x00100000  && (uint32)memNode[i].addr <= endOfReservedMemory ) 
			{
				//Move everything away from the kernel and modules.
				node = (memoryManager_freeMemoryNode*) endOfReservedMemory;

				node->address = (uint64) endOfReservedMemory + sizeof(memoryManager_freeMemoryNode);
				node->length = (uint64) memNode[i].len - sizeof(memoryManager_freeMemoryNode) - (endOfReservedMemory - 0x00100000);
			} else {
				//Don't need to move anything.
				node = (memoryManager_freeMemoryNode*) (uint32) memNode[i].addr; //Put a free block in the start of this area.

				node->address = (uint64) memNode[i].addr + sizeof(memoryManager_freeMemoryNode);
				node->length = (uint64) memNode[i].len - sizeof(memoryManager_freeMemoryNode);
			}

			//Do magic to try and find where to place this in the list.
			if((uint32)memoryManager_firstFreeNode==END_OF_MEMORY_LIST)
			{
				node->next = (memoryManager_freeMemoryNode*) END_OF_MEMORY_LIST;
				memoryManager_firstFreeNode = node;
			} else {
				//Add it to the front of the list. We'll figure out sorting it later.
				node->next = memoryManager_firstFreeNode;
				memoryManager_firstFreeNode = node;
			}
		}
	}
	
	//Print this out, just for debugging purposes.
	memoryManager_debug_printFreeMemoryList();
}

/**
 * Print out the complete list of free memory regions in the system.
 */
void memoryManager_debug_printFreeMemoryList(void)
{
	uint32 total = 0;
	
	memoryManager_freeMemoryNode* current = memoryManager_firstFreeNode;
	while((uint32) current != END_OF_MEMORY_LIST)
	{
		vgaConsole_printf("Free memory block: %h (start) ... %h (length)\n",(uint32)current->address, (uint32)current->length);
		
		total += current->length;
		
		current = current->next;
	}
	
	vgaConsole_printf("Available memory:\t%h\n",total);
}