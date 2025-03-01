#include <physicalMemory.h>
#include <memory.h>

/**
 * The virtual (so we can access it) address of the physical memory allocation bitmap.
 */
#define PHYSICALMEMORY_BITMAPADDRESS    0xC0060000

/**
 * The size of the physical memory allocation bitmap, in bytes.
 */
#define PHYSICALMEMORY_BITMAPSIZE       0x20000

uint32 physicalMemory_findEndOfReservedMemory(struct multiboot_moduleNode* module, uint32 count) {
    uint32 endOfReservedMemory = (uint32) &_kernel_end;

    for (uint32 i = 0; i < count; i++) {
        if (module[i].end > (void*) endOfReservedMemory) {
            endOfReservedMemory = (uint32) module[i].end;
        }
    }

    return endOfReservedMemory;
}

/**
 * Call this initialisation AFTER paging and GDT, but before any other memory stuff (virtual or heap).
 */

void physicalMemory_initialise(struct multiboot_info* multibootData) {
    // the easiest way to do this is to mark the whole area as allocated and then free the bits that are ram.

    // Mark 128KiB of RAM at physical 0x60000 with zeroes (representing allocated memory).
    memzero(PHYSICALMEMORY_BITMAPADDRESS, PHYSICALMEMORY_BITMAPSIZE);

    // Mark all available RAM as free.
    for(uint32 i = 0; i < (multibootData->memoryMapLength / 20) - 1; i++) {
        struct multiboot_memoryMapNode mem = (multibootData->memoryMapAddress[i]);

        uint64 end = mem.addr + mem.len - 1;

        if(mem.type == 1) {
            // mark free.

            uint32 pages = mem.len >> 12;

            // TODO(JackScottAU): Make this something other than the least efficient way possible of doing this.
            for(int j = 0; j < pages; j++) {
                physicalMemory_free(mem.addr);
            }
        }
    }

    // Mark all RAM below end of kernel/modules as allocated.
    uint32 end = physicalMemory_findEndOfReservedMemory(multibootData->modsAddr, multibootData->modsCount);
    
    uint32 mpages = end >> 12;

    // TODO(JackScottAU): Make this something other than the least efficient way possible of doing this.
    for(int j = 0; j < mpages; j++) {
        physicalMemory_markAllocated(j << 12);
    }
}

void* physicalMemory_allocate() {
    uint8* bitmap = (uint8*)PHYSICALMEMORY_BITMAPADDRESS;

    // TODO: hold a persistent index to the first byte with free memory to speed up this search.

    for(uint32 i = 0; i < PHYSICALMEMORY_BITMAPSIZE; i++) { // iterate through 128K of memory.
   //     debug(LOGLEVEL_TRACE, "Index %d: %h", i, bitmap[i]);

        if(bitmap[i] != 0x00) {

            for(uint8 b = 0; b < 8; b++) {
                if((bitmap[i] >> b) & 0x01) {
                    // the b-th bit of this index is free, we can return that.
                    uint8 mask = 0x01 << b;
                    bitmap[i] = bitmap[i] & !mask;

                    return ((i * 8) + b) << 12;
                }
            }

          //  return (i * 8) << 12;
        }
    }

    return 0xFFFFFFFF; // no memory found.
}

// not sure this is actually needed?
void physicalMemory_markAllocated(void* address) {
    uint8* bitmap = (uint8*)PHYSICALMEMORY_BITMAPADDRESS;

    uint32 index = ((uint32)address >> 12) / 8;
    uint32 bit = ((uint32)address >> 12) % 8;

    uint8 mask = 0x01 << bit;

    bitmap[index] = bitmap[index] & !mask;
}

void physicalMemory_free(void* address) {
    uint8* bitmap = (uint8*)PHYSICALMEMORY_BITMAPADDRESS;

    uint32 index = ((uint32)address >> 12) / 8;
    uint32 bit = ((uint32)address >> 12) % 8;

    uint8 mask = 0x01 << bit;

    bitmap[index] = bitmap[index] | mask;   
}
