// Synergy OS

// INCLUDES
#include <multiboot.h>
#include <Types.h>
#include <GDT.h>
#include <vgaConsole.h>
#include <interrupts.h>
#include <Clock.h>
#include <portIO.h>
#include <memoryManager.h>

extern uint32 kernel_end;
uint32 memoryManager_findEndOfReservedMemory(struct multiboot_moduleNode* module, uint32 count);

uint32 memoryManager_findEndOfReservedMemory(struct multiboot_moduleNode* module, uint32 count)
{
	uint32 endOfReservedMemory = (uint32) &kernel_end;
	
	for(uint32 i = 0; i < count; i++)
	{
		if(module[i].end > (void*) endOfReservedMemory)
			endOfReservedMemory = (uint32) module[i].end;
	}
	
	return endOfReservedMemory;
}

//To shut GCC up.
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData);

/**
 * Initialises the core systems of the kernel and language runtime before launching a command interpreter.
 * @param magicNumber The check number passed from the multiboot loader.
 * @param multibootData The info data structure passed from the multiboot loader.
 */
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData)
{
	if(magicNumber != 0x2BADB002)
	{
		vgaConsole_printf("Multiboot error found. Halting...");
		interrupts_disableInterrupts();
		haltCPU();
	}
 
	vgaConsole_clearScreen();
	
	vgaConsole_printf("Synergy is booting...\n\n");
	
	vgaConsole_printf("Multiboot Flags: \t%h\n",multibootData->flags);
	
	vgaConsole_printf("Loading GDT...\t\t\t\t\t\t\t\t");
	gdt_install();
	vgaConsole_printf("%s",1);
	
	vgaConsole_printf("Setting up Interrupts...\t\t\t\t\t\t");
	interrupts_initialise();
	vgaConsole_printf("%s",1);
	
	memoryManager_init(multibootData->memoryMapAddress, multibootData->memoryMapLength, (uint32) memoryManager_findEndOfReservedMemory(multibootData->modsAddr, multibootData->modsCount));
	
	clock_init();
	
	void* foo;
	foo = memoryManager_allocate(0x10);
	vgaConsole_printf("Allocated at: %h\n",foo);
	memoryManager_debug_printFreeMemoryList();
	
	
	while(1)
	{
		//Do Nothing here
	};
}

