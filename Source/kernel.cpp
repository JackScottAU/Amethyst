/**
 *  Amethyst Operating System - Core kernel initialisation.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Clock.h>
#include <GDT.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <multiboot.h>
#include <portIO.h>
#include <Types.h>
#include <vgaConsole.h>
#include <pciBus.h>
#include <keyboard.h>
#include <serial.h>
#include <stream.h>
#include <string.h>
#include <deviceTree.h>
#include <ps2controller.h>
#include <debug.h>
#include <amethyst.h>
#include <mouse.h>
#include <thread.h>

#include <Structures/linkedlist.hpp>

#include <shell.hpp>
#include <StandardIO.hpp>

#ifdef    __cplusplus
extern "C" {
#endif

thread_control_block* current_task_TCB = (thread_control_block*) 0xC0001000;
thread_control_block* task1;

// To shut GCC up.
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData);
struct multiboot_info* multiboot_correctDataStructureAddresses(struct multiboot_info* data);
void kernel_printBanner(void (*putChar)(char));
uint32 memoryManager_printPhysicalMemoryMap(StandardIO* stdio);
uint32 printCurrentTask(StandardIO* stdio);

void testfunc() {
    interrupts_enableInterrupts();

    while(1) {
        debug(LOGLEVEL_INFO, "Hello from thread #2. cr3 = %h", task1->cr3);

        haltCPU();
    }
}

struct multiboot_info* multiboot_correctDataStructureAddresses(struct multiboot_info* data) {
    data = (struct multiboot_info*)(((uint32) data) + 0xC0000000);

    data->modsAddr = (multiboot_moduleNode*)((uint32)(data->modsAddr) + (uint32)0xC0000000);
    data->memoryMapAddress = (multiboot_memoryMapNode*)((uint32)(data->memoryMapAddress) + (uint32)0xC0000000);

    // For each multiboot module, update the provided (physical) address so it is in (logical) kernel memory according
    // to our paging rules.
    for (uint32 i = 0; i < data->modsCount; i++) {
        data->modsAddr[i].start = (void*)((uint32)data->modsAddr[i].start + 0xC0000000);
        data->modsAddr[i].end = (void*)((uint32)data->modsAddr[i].end + 0xC0000000);
        data->modsAddr[i].fileName = (char*)((uint32)data->modsAddr[i].fileName + 0xC0000000);
    }

    return data;
}

void kernel_printBanner(void (*putChar)(char)) {
    stream_printf(putChar, "\033[35m");

    stream_printf(putChar, "\n\t           _                   _   _               _   \n");
    stream_printf(putChar, "\t          / \\   _ __ ___   ___| |_| |__  _   _ ___| |_ \n");
    stream_printf(putChar, "\t         / _ \\ | '_ ` _ \\ / _ \\ __| '_ \\| | | / __| __|\n");
    stream_printf(putChar, "\t        / ___ \\| | | | | |  __/ |_| | | | |_| \\__ \\ |_ \n");
    stream_printf(putChar, "\t       /_/   \\_\\_| |_| |_|\\___|\\__|_| |_|\\__, |___/\\__|\n");
    stream_printf(putChar, "\t                                         |___/         \n");

    stream_printf(putChar, "\033[0m");

    stream_printf(putChar, "\t\t\t\tVersion %d.%d.%d\n\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}

multiboot_info* multiBootDataP;

/**
 * Initialises the core systems of the kernel and language runtime before launching a command interpreter.
 * @param magicNumber The check number passed from the multiboot loader.
 * @param multibootData The info data structure passed from the multiboot loader.
 */
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData) {
    vgaConsole_initialise();
  //  vgaConsole_clearScreen();

    // Before we can do anything else, we need to do three things:
    // 1.  Check the multiboot data for a valid boot environment.
    // 2.  Set up a basic IDT and GDT.
    // 3.  Set up the kernel memory managers (page-level and heap-level).
    // Once this has been done, we can begin using the debug() functionality and start setting up devices.

    debug(LOGLEVEL_INFO, "Checking Multiboot data...\n");
    if (magicNumber != MULTIBOOT_MAGIC_NUMBER) {
        stream_printf(vgaConsole_putChar, "\nMultiboot error found. Halting...");
        interrupts_disableInterrupts();
        haltCPU();
    } else {
        multibootData = multiboot_correctDataStructureAddresses(multibootData);
    }

    multiBootDataP = multibootData;

    stream_printf(vgaConsole_putChar, "Loading a GDT...\n");
    gdt_install();

    stream_printf(vgaConsole_putChar, "Setting up interrupts...\n");
    interrupts_initialise();

    stream_printf(vgaConsole_putChar, "Setting up the memory manager...\n");
    memoryManager_init(multibootData->memoryMapAddress, multibootData->memoryMapLength,
        (uint32) memoryManager_findEndOfReservedMemory(multibootData->modsAddr, multibootData->modsCount));

    stream_printf(vgaConsole_putChar, "Enumerating PCI buses...\n");
    pci_enumerateBuses();

    serial_init(SERIAL_COM1, SERIAL_BAUD_38400);

    // Testing SGR...
    kernel_printBanner(vgaConsole_putChar);
    kernel_printBanner(serial_writeChar);

    serial_writeLine("Amethyst Debugging Information:\n");

    debug(LOGLEVEL_INFO, "Setting up the clock...");
    clock_init();

    deviceTree_build();

    // Initialise the standard I/O streams for use by the shell.
    StandardIO* console = new StandardIO(vgaConsole_putChar, keyboard_readChar);
    console->Print("\n");

    
    initialise_multitasking();

    task1 = current_task_TCB;
    thread_control_block* task2 = new_task(testfunc, task1);

    task1->nextThread = task2;
    task2->nextThread = task1;

 //   while(true) {
 //       debug(LOGLEVEL_INFO, "Hello from thread #1. cr3 = %h", task1->cr3);
 //       switch_to_task(task2);
 //   }

    thread_startScheduler();

    debug(LOGLEVEL_INFO, "Got here");

    interrupts_enableInterrupts();

    // Launch the kernel shell.
    Shell* shell = new Shell(console);
    shell->RegisterCommand("Print-PhysicalMemoryMap", memoryManager_printPhysicalMemoryMap);
    shell->RegisterCommand("ct", printCurrentTask);
    shell->Main();

    delete shell;
    delete console;
}



uint32 printCurrentTask(StandardIO* stdio) {
    stdio->Print("CR3: %h\n", current_task_TCB->cr3);
    stdio->Print("ESP: %h\n", current_task_TCB->kernel_stack_top);

    return 42;
}

uint32 memoryManager_printPhysicalMemoryMap(StandardIO* stdio) {
    for(uint32 i = 0; i < (multiBootDataP->memoryMapLength / 20); i++) {
        multiboot_memoryMapNode mem = (multiBootDataP->memoryMapAddress[i]);

        uint64 end = mem.addr + mem.len - 1;

        stdio->Print("addrl: %H\tlen: %H\tend: %H\ttype: %d\n"
        , (uint32) mem.addr
        , (uint32) mem.len
        , (uint32) end
        , mem.type);
        
    }

    return 42;
}

#ifdef    __cplusplus
}
#endif
