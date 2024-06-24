/**
 *  Amethyst Operating System - Memory management functions.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <memoryManager.h>
#include <multiboot.h>
#include <vgaConsole.h>        // This is only used for debugging.


// Holds the address of the start of the list representing free memory blocks.
// When the kernel is first started, this list is empty.
memoryManager_freeMemoryNode* memoryManager_firstFreeNode = (memoryManager_freeMemoryNode*) END_OF_MEMORY_LIST;


/**
 * Allocates a block of memory of the correct size. Currently uses a first-fit algorithm.
 * @param size The size of the requested block of memory.
 * @return A pointer to the block of memory.
 */
void* memoryManager_allocate(uint32 size) {
    // TODO(Jack): Switch from a first-fit to a best-fit algorithm.

    // A pointer to the header that is before the allocated block.
    // We allocate space for this header once we have found a free space for it.
    memoryManager_allocatedHeader* memoryHeader;

    // A pointer to the list of free memory spaces.
    memoryManager_freeMemoryNode* current = memoryManager_firstFreeNode;

    // Once we have traversed the list, this pointer holds the best free memory space.
    memoryManager_freeMemoryNode* chosen = (memoryManager_freeMemoryNode*) END_OF_MEMORY_LIST;

    // If the size requested is small enough that it can't be freed safely, make it so it is.
    if (size < (sizeof(memoryManager_freeMemoryNode) + 4))
        size = sizeof(memoryManager_freeMemoryNode) + 4;

    // Traverse the list.
    while ((uint32) current != END_OF_MEMORY_LIST) {
        if (current->length > (size+sizeof(memoryManager_allocatedHeader))) {
            // We've found a good space. Use it.
            chosen = current;
            break;
        }

        current = current->next;
    }

    // We now have a free block of memory. chosen->address points to the start of this area.

    // Create the allocated memory header at the start of the free area.
    memoryHeader = (memoryManager_allocatedHeader*) (uint32) chosen->address;

    // In the header we store how much was allocated. Maybe we'll store other things later?
    memoryHeader->size = size;

    // Update the address and length of the free area accordingly.
    chosen->address = chosen->address + size + sizeof(memoryManager_allocatedHeader);
    chosen->length = chosen->length - size - sizeof(memoryManager_allocatedHeader);

    // Return the first usable byte of the allocated block.
    return (void*) ((uint32)memoryHeader + (uint32)sizeof(memoryManager_allocatedHeader));
}

/**
 * Frees a block of memory that was allocated using memoryManager_allocate().
 * @param mem The first usable byte (after the header) of the allocated memory block.
 */
void memoryManager_free(void* mem) {
    memoryManager_allocatedHeader* allocatedHeader =
        (memoryManager_allocatedHeader*) ((uint32)mem- sizeof(memoryManager_allocatedHeader));

    int size = allocatedHeader->size + sizeof(memoryManager_allocatedHeader);

    memoryManager_freeMemoryNode* node =
        (memoryManager_freeMemoryNode*) ((uint32)mem- sizeof(memoryManager_allocatedHeader));

    node->address = (uint32) node + sizeof(memoryManager_freeMemoryNode);
    node->length = size - sizeof(memoryManager_freeMemoryNode);

    // Do magic to try and find where to place this in the list.
    if ((uint32)memoryManager_firstFreeNode == END_OF_MEMORY_LIST) {
        node->next = (memoryManager_freeMemoryNode*) END_OF_MEMORY_LIST;
        memoryManager_firstFreeNode = node;
    } else {
        // Add it to the front of the list. We'll figure out sorting it later.
        node->next = memoryManager_firstFreeNode;
        memoryManager_firstFreeNode = node;
    }

/*    vgaConsole_printf("Freeing memory:\n\t");
    vgaConsole_printf("Value given to function:\t%h\n\t",mem);
    vgaConsole_printf("Allocated header address:\t%h\n\t",allocatedHeader);
    vgaConsole_printf("sizeof(memoryManager_allocatedHeader):\t%h\n\t",sizeof(memoryManager_allocatedHeader));
    vgaConsole_printf("sizeof(memoryManager_freeMemoryNode):\t%h\n\t",sizeof(memoryManager_freeMemoryNode));
    vgaConsole_printf("Size:\t%h\n\t",size);
    vgaConsole_printf("node->address:\t%h\n\t",node->address);
    vgaConsole_printf("node->length:\t%h\n\t",node->length);
    vgaConsole_printf("node->next:\t%h\n\t",node->next);
    vgaConsole_printf("node:\t%h\n\t",node);
    vgaConsole_printf("memoryManager_firstFreeNode:\t%h\n\t",memoryManager_firstFreeNode);
    */
}

/**
 * Uses the map of memory supplied by the multiboot loader to create a list of free memory.
 * @param memNode An array of all the memory map records passed from the multiboot loader.
 * @param length The length in bytes of the memory map array.
 * @param endOfReservedMemory A pointer to the end of the kernel (or module's) code, so we don't overwrite it.
 */
void memoryManager_init(struct multiboot_memoryMapNode* memNode, uint32 length, uint32 endOfReservedMemory) {
    // fake it for now!
    memoryManager_firstFreeNode = (memoryManager_freeMemoryNode*)0xC0007C00;

    memoryManager_firstFreeNode->address = 0xC0200000;      // 2 megs.
    memoryManager_firstFreeNode->length = 0x200000;         // 2 megs.
    memoryManager_firstFreeNode->next = (memoryManager_freeMemoryNode*)END_OF_MEMORY_LIST;


    /*memNode = (uint32) memNode + 0xC0000000;

    vgaConsole_printf("memNode: %h\n", memNode);

    // Loop through each of the memory map records. Each record is 24 bytes long.
    for(uint32 i=0; i<length/24; i++)
    {
        
            vgaConsole_printf("memnode size: %h\n", memNode[i].len);

            vgaConsole_printf("memnode address: %h\n", memNode[i].addr);
            vgaConsole_printf("memnode type: %h\n\n", memNode[i].type);

        //We need to check if it is available RAM (type 1) and that the size is worth recording.
        if(memNode[i].type==1 && memNode[i].len>sizeof(memoryManager_freeMemoryNode))
        {
            //We need to be able to address the free memory record somehow.
            memoryManager_freeMemoryNode* node;

            vgaConsole_printf("0: %h\n", memNode[i].addr);    
                
            //Check if we're overwriting kernels or modules.
            if( (uint32)memNode[i].addr >= 0x00100000  && (uint32)memNode[i].addr <= endOfReservedMemory ) 
            {
                vgaConsole_printf("1: %h\n", memNode[i].addr);


                //Move everything away from the kernel and modules.
                node = (memoryManager_freeMemoryNode*) (((uint32) endOfReservedMemory));
                vgaConsole_printf("4: %h\n", node);

                node->address = (uint64) endOfReservedMemory + sizeof(memoryManager_freeMemoryNode) + 0xC0000000;
                node->length = (uint64) memNode[i].len - sizeof(memoryManager_freeMemoryNode) - (endOfReservedMemory - 0x00100000);

                vgaConsole_printf("5: %h %h\n", node->address, node->length);

            } else {
                vgaConsole_printf("2: %h\n", memNode[i].addr);

                //Don't need to move anything.
                node = (memoryManager_freeMemoryNode*) (((uint32) memNode[i].addr) + 0xC0000000); //Put a free block in the start of this area.

                vgaConsole_printf("3: %h\n", node);

                node->address = (uint64) memNode[i].addr + sizeof(memoryManager_freeMemoryNode) + 0xC0000000;
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
    //memoryManager_debug_printFreeMemoryList();*/
}

uint32 memoryManager_findEndOfReservedMemory(struct multiboot_moduleNode* module, uint32 count) {
    uint32 endOfReservedMemory = (uint32) &_kernel_end;

    for (uint32 i = 0; i < count; i++) {
        if (module[i].end > (void*) endOfReservedMemory) {
            endOfReservedMemory = (uint32) module[i].end;
        }
    }

    return endOfReservedMemory;
}

/**
 * Print out the complete list of free memory regions in the system.
 */
void memoryManager_debug_printFreeMemoryList(void) {
    uint32 total = 0;

    memoryManager_freeMemoryNode* current = memoryManager_firstFreeNode;
    while ((uint32) current != END_OF_MEMORY_LIST) {
        vgaConsole_printf("Free memory block: %h (start) ... %h (length) (%h)\n",
            (uint32)current->address, (uint32)current->length, current);

        total += current->length;

        current = current->next;
    }

    vgaConsole_printf("Available memory:\t%h\n", total);
}
