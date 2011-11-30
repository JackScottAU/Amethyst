#include <multiboot.h>
#include <vgaConsole.h>
#include <memoryManager.h>

memoryManager_freeMemoryNode* memoryManager_firstFreeNode = (memoryManager_freeMemoryNode*) 0xFFFFFFFF;


void memoryManager_init(struct multiboot_memoryMapNode* startAddress, uint32 length)
{
	struct multiboot_memoryMapNode* memNode = (void*) startAddress;
	
	for(uint32 i=0; i<length/24; i++)
	{
		//Print details
		if(memNode[i].type==1)
		{
			vgaConsole_printf("%h.%h.%h\n",(uint32)memNode[i].addr, (uint32)memNode[i].len,memNode[i].type);
			
			if(memNode[i].len>sizeof(memoryManager_freeMemoryNode))
			{
				memoryManager_freeMemoryNode* node = (memoryManager_freeMemoryNode*) (uint32) memNode[i].addr; //Put a free block in the start of this area.
				node->address = (uint64) memNode[i].addr + sizeof(memoryManager_freeMemoryNode);
				node->length = (uint64) memNode[i].len - sizeof(memoryManager_freeMemoryNode);
				
				//Do magic to try and find where to place this in the list.
				if((uint32)memoryManager_firstFreeNode==0xFFFFFFFF)
				{
					node->next = (memoryManager_freeMemoryNode*) 0xFFFFFFFF;
					memoryManager_firstFreeNode = node;
				} else {
					//Add it to the front of the list. We'll figure out sorting it later.
					node->next = memoryManager_firstFreeNode;
					memoryManager_firstFreeNode = node;
				}
			} else {
				//PANIC.
			}
		}
	}
	
	
	//Display list of nodes for testing.
	memoryManager_freeMemoryNode* current = memoryManager_firstFreeNode;
	while((uint32) current != 0xFFFFFFFF)
	{
		vgaConsole_printf("In list: %h.%h\n",(uint32)current->address, (uint32)current->length);
		
		current = current->next;
	}
}
