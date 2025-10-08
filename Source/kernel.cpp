/**
 *  Amethyst Operating System - Core kernel initialisation.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Clock.h>
#include <GDT.h>
#include <interrupts.h>
#include <physicalMemory.h>
#include <memoryManager.h>
#include <multiboot.h>
#include <portIO.h>
#include <Types.h>
#include <stream.h>
#include <string.h>
#include <deviceTree.h>
#include <debug.h>
#include <amethyst.h>
#include <thread.h>

#include <Drivers/qemuVga.h>
#include <Drivers/serial.h>
#include <Drivers/ps2controller.h>
#include <Drivers/pciBus.h>
#include <Drivers/keyboard.h>
#include <Drivers/mouse.h>
#include <Drivers/atiRage128.h>

#include <Graphics/canvas.h>
#include <Graphics/TextConsole.hpp>
#include <Graphics/TextLabel.hpp>
#include <Graphics/TargaImage.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/Desktop.hpp>

#include <Structures/linkedlist.hpp>

#include <shell.hpp>
#include <StandardIO.hpp>

#ifdef    __cplusplus
extern "C" {
#endif

// To shut GCC up.
void kernel_initialise(uint32 magicNumber, struct multiboot_info* multibootData);
struct multiboot_info* multiboot_correctDataStructureAddresses(struct multiboot_info* data);
void kernel_printBanner(void (*putChar)(char));
uint32 memoryManager_printPhysicalMemoryMap(StandardIO* stdio);

TextConsole* stdioTextBox;

Widget* rootWidget;

void textBoxPutChar(char c) {
    stdioTextBox->PutChar(c);
}

void startShell() {
    interrupts_enableInterrupts();

    // Initialise the standard I/O streams for use by the shell.
    StandardIO* console = new StandardIO(textBoxPutChar, keyboard_readChar);
    console->Print("\n");
    // Launch the kernel shell.
    Shell* shell = new Shell(console);
    shell->RegisterCommand("Print-PhysicalMemoryMap", memoryManager_printPhysicalMemoryMap);
    shell->Main();

    delete shell;
    delete console;
}

void startSerialShell() {
    interrupts_enableInterrupts();

    // Initialise the standard I/O streams for use by the shell.
    StandardIO* console = new StandardIO(serial_writeChar, serial_readChar);
    console->Print("\n");
    // Launch the kernel shell.
    Shell* shell = new Shell(console);
    shell->RegisterCommand("Print-PhysicalMemoryMap", memoryManager_printPhysicalMemoryMap);
    shell->Main();

    delete shell;
    delete console;
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
    // Before we can do anything else, we need to do three things:
    // 1.  Check the multiboot data for a valid boot environment.
    // 2.  Set up a basic IDT and GDT.
    // 3.  Set up the kernel memory managers (page-level and heap-level).
    // Once this has been done, we can begin using the debug() functionality and start setting up devices.

    debug(LOGLEVEL_INFO, "Checking Multiboot data...\n");
    if (magicNumber != MULTIBOOT_MAGIC_NUMBER) {
        debug(LOGLEVEL_CRITICAL, "\nMultiboot error found. Halting...");
        interrupts_disableInterrupts();
        haltCPU();
    } else {
        multibootData = multiboot_correctDataStructureAddresses(multibootData);
    }

    multiBootDataP = multibootData;

    debug(LOGLEVEL_INFO, "Loading a GDT...\n");
    gdt_install();

    debug(LOGLEVEL_INFO, "Setting up interrupts...\n");
    interrupts_initialise();

    debug(LOGLEVEL_INFO, "Setting up the memory manager...\n");
    physicalMemory_initialise(multibootData);
    memoryManager_init(multibootData->memoryMapAddress, multibootData->memoryMapLength,
        (uint32) physicalMemory_findEndOfReservedMemory(multibootData->modsAddr, multibootData->modsCount));

    serial_init(SERIAL_COM1, SERIAL_BAUD_38400);

    // Testing SGR...
  //  kernel_printBanner(vgaConsole_putChar);
    kernel_printBanner(serial_writeChar);

    serial_writeLine("Amethyst Debugging Information:\n");

    debug(LOGLEVEL_INFO, "Setting up the clock...");
    clock_init();

    PageDirectory* pg = memoryManager_getCurrentPageDirectory();

    memoryManager_mapPhysicalMemoryPage(pg, (void*)0xC0400000, (void*)0x00400000, 1024);    // we now have another 4 megs to play with!

    deviceTree_build();

    // We now have a QEMU display adapter somewhere in the device tree, and it knows where it is, so we can use it.
    // qemuVga_setMode(1024, 768);
    Canvas* canvas = atiRage128_getCanvas();

    multiboot_moduleNode* modules = multibootData->modsAddr;

    ScreenFont* font = (ScreenFont*)memoryManager_allocate(sizeof(ScreenFont));
    font->header = (ScreenFontHeader*)(modules[0].start);
    font->characterData = (uint8*)(font->header) + font->header->headerSize;

    ScreenFont* font2 = (ScreenFont*)memoryManager_allocate(sizeof(ScreenFont));
    font2->header = (ScreenFontHeader*)(modules[2].start);
    font2->characterData = (uint8*)(font2->header) + font2->header->headerSize;

    uint32 length = (uint32)modules[1].end - (uint32)modules[1].start;
    debug(LOGLEVEL_CRITICAL, "LENGTH: %h", length);

    TargaImage* image = new TargaImage((uint8*)modules[1].start, length, 200, 300, canvas);

    Desktop* desktop = new Desktop(canvas);
    rootWidget = desktop;

    Window* window = new Window(font2, 0, 0, 1024, 768, canvas, "Amethyst OS");

    desktop->AddChild(window);

    stdioTextBox = new TextConsole(canvas, font, 0, 32, 46, 128);

    window->AddChild(stdioTextBox);
    //   window->AddChild(image);

    for (int i = 0; i < 1000; i++) {
        image->SetPosition(i, 100);
    //  image->Redraw();
    }

    //  uint32 pageaddress = memoryManager_getPhysicalAddressOfFreePhysicalPage();
    //  debug(LOGLEVEL_ERROR, "page address: %h", pageaddress);

    // Sets up the initial TCB.
    thread_control_block* task1 = initialise_multitasking();

    // Start shells in new threads.
    thread_control_block* task2 = new_task(startShell, task1);
//  thread_control_block* task3 = new_task(startSerialShell, task1);

    // Because our scheduler is very stupid, we do this.
    task1->nextThread = task2;
    task2->nextThread = task1;
//  task3->nextThread = task1;

    thread_startScheduler();

    debug(LOGLEVEL_INFO, "Got here");

    interrupts_enableInterrupts();

    // The initial startup thread now becomes the system idle task for this CPU.
    while (1) {
        debug(LOGLEVEL_TRACE, "System idle task running.");

    //    debug(LOGLEVEL_DEBUG, "CR3: %h\n", memoryManager_getCurrentPageDirectory());

        haltCPU();
    }
}

void sortOfMouse_HandleEvent(sint16 moveX, sint16 moveY) {
    MouseMoveEvent* event = new MouseMoveEvent(moveX, moveY);

    rootWidget->HandleUIEvent(event);
}

void sortOfMouse_HandleClickEvent() {
    MouseClickEvent* event = new MouseClickEvent();

    rootWidget->HandleUIEvent(event);
}

uint32 memoryManager_printPhysicalMemoryMap(StandardIO* stdio) {
    for (uint32 i = 0; i < (multiBootDataP->memoryMapLength / 20) - 1; i++) {
        multiboot_memoryMapNode mem = (multiBootDataP->memoryMapAddress[i]);

        uint64 end = mem.addr + mem.len - 1;

        const char* type;

        switch (mem.type) {
            case 1:
                type = "Available RAM";
                break;

            case 3:
                type = "ACPI";
                break;

            case 4:
                type = "Hibernation RAM";
                break;

            case 5:
                type = "Defective RAM";
                break;

            default:
                type = "System Reserved";
                break;
        }

        stdio->Print("addrl: %H\tlen: %H\tend: %H\ttype: %d (%s)\n", (uint32) mem.addr, (uint32) mem.len, (uint32) end, mem.type, type);
    }

    return 42;
}

#ifdef    __cplusplus
}
#endif
