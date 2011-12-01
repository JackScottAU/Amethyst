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


//To shut GCC up.
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData);
void kernel_testClock(void);

clock_timerRequest* grrr;

void kernel_testClock(void)
{
	vgaConsole_printf("Clock shot! Time: %h\n",clock_uptime());
	
	//clock_deleteTimerRequest(grrr);
}

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
	
	vgaConsole_printf("Loading a GDT...\t\t\t\t\t\t\t");
	gdt_install();
	vgaConsole_printf("%s",1);
	
	vgaConsole_printf("Setting up interrupts...\t\t\t\t\t\t");
	interrupts_initialise();
	vgaConsole_printf("%s",1);
	
	vgaConsole_printf("Setting up the memory manager...\t\t\t\t\t");
	memoryManager_init(multibootData->memoryMapAddress, multibootData->memoryMapLength, (uint32) memoryManager_findEndOfReservedMemory(multibootData->modsAddr, multibootData->modsCount));
	vgaConsole_printf("%s",1);
	
	vgaConsole_printf("Setting up the clock...\t\t\t\t\t\t\t");
	clock_init();
	vgaConsole_printf("%s",1);
	
	void* foo;
	foo = memoryManager_allocate(0x10);
	vgaConsole_printf("Allocated at: %h\n",foo);
	memoryManager_debug_printFreeMemoryList();
	vgaConsole_printf("About to free.\n");
	memoryManager_free(foo);
	
	grrr = clock_addRepeatRequest(1, 0, (*kernel_testClock));
	
	while(1)
	{
		//Do Nothing here
	};
}

