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

#include <shell.hpp>
#include <StandardIO.hpp>

#ifdef    __cplusplus
extern "C" {
#endif

// To shut GCC up.
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData);
struct multiboot_info* multiboot_correctDataStructureAddresses(struct multiboot_info* data);
void kernel_printBanner(void (*putChar)(char));

struct multiboot_info* multiboot_correctDataStructureAddresses(struct multiboot_info* data) {
    data = (struct multiboot_info*)(((uint32) data) + 0xC0000000);

    data->modsAddr = (multiboot_moduleNode*)((uint32)(data->modsAddr) + (uint32)0xC0000000);

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

    ps2controller_initialise();
    deviceTree_build();

    stream_printf(vgaConsole_putChar, "\n");

    // Initialise the standard I/O streams for use by the shell.
    StandardIO* console = new StandardIO(vgaConsole_putChar, keyboard_readChar);

    // Launch the kernel shell.
    Shell* shell = new Shell(console);
    shell->Main();

    delete shell;
    delete console;
}

#ifdef    __cplusplus
}
#endif
