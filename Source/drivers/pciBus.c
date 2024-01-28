/**
 * Enumerates PCI buses. Will do more eventually.
 * The reference for most of the code within this file is http://wiki.osdev.org/PCI
 */

#include <Types.h>
#include <portIO.h>
#include <vgaConsole.h>
#include <pciBus.h>
#include <memoryManager.h>
#include <deviceTree.h>
#include "pci/deviceNames.h"

 // PCI Bus Class names. If we can get these within 34 characters length we can use in display table below.
const char* classNames[] = {
    "Unknown                           ",
    "Mass Storage Controller           ",
    "Network Controller                ",
    "Display Controller                ",
    "Multimedia Controller             ",
    "Memory Controller                 ",
    "Bridge Device                     ",
    "Simple Communications Controller  ",
    "Base System Peripheral            ",
    "Input Device                      ",
    "Docking Station                   ",
    "Processor                         ",
    "Serial Bus Controller             ",
    "Wireless Controller               ",
    "Intelligent I/O Controller        ",
    "Satellite Communication Controller",
    "Encryption/Decryption Controller  ",
    "Signal Processing Controller      ",
    "Reserved                          "
};

pciBus_Entry* pci_busEntries;
pciBus_Entry* pci_currentEntry;

// There are 256 buses, each with up to 32 devices (need to rename us from slots to devices), each with up to 8 functions.

deviceTree_Entry* pci_addDevicesToTree(void) {
    pci_currentEntry = pci_busEntries;

    // Assume the first device is the root hub.
    deviceTree_Entry* root = memoryManager_allocate(sizeof(deviceTree_Entry));
    root->next = NULL;
    root->child = NULL;
    root->name = pci_getNameFromVendorAndDevice(pci_currentEntry->vendorID, pci_currentEntry->deviceID);

    pci_currentEntry = pci_currentEntry->next;
    while (pci_currentEntry->next != 0x0)
    {
        char* name = pci_getNameFromVendorAndDevice(pci_currentEntry->vendorID, pci_currentEntry->deviceID);

        deviceTree_Entry* device = deviceTree_createDevice(name, DEVICETREE_TYPE_PCI, pci_currentEntry);

        deviceTree_addChild(root, device);

        pci_currentEntry = pci_currentEntry->next;
    }

    return root;
}



void pci_printBuses(void)
{
    vgaConsole_printf("  +-----+------+------+--------+--------+------------------------------------+\n");
    vgaConsole_printf("  | BUS | SLOT | FUNC | VENDOR | DEVICE | CLASS DESCRIPTION                  |\n");
    vgaConsole_printf("  +-----+------+------+--------+--------+------------------------------------+\n");

    // Iterate through the bus entries stored.
    pci_currentEntry = pci_busEntries;
    while (pci_currentEntry->next != 0x0)
    {
        // Print entry.
        vgaConsole_printf("  | %d   | %d    | %d    | %h | %h | ", pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function, pci_currentEntry->vendorID, pci_currentEntry->deviceID);
        vgaConsole_printf(classNames[pci_currentEntry->classID]);
        vgaConsole_printf(" |\n");

        pci_currentEntry = pci_currentEntry->next;
    }

    vgaConsole_printf("  +-----+------+------+--------+--------+------------------------------------+\n");
}

void pci_enumerateBuses(void) {
    uint16 bus;

    //very brute force. need to fix this.

    // Init the table.
    pci_busEntries = memoryManager_allocate(sizeof(pciBus_Entry));
    pci_currentEntry = pci_busEntries;
    pci_currentEntry->next = 0x0; // set next to zero. this is added later.

    for (bus = 0; bus <= 255; bus++) {
        pci_checkBus(bus);
    }
}

void pci_checkBus(uint8 bus)
{
    uint8 slot;

    for (slot = 0; slot < 32; slot++)
        pci_checkSlot(bus, slot);
}

void pci_checkSlot(uint8 bus, uint8 slot) {
    uint8 function = 0;

    for (function = 0; function < 8; function++) {
        uint32 deviceAndVendor = pci_readConfigurationRegister(bus, slot, function, 0x00);

        if ((deviceAndVendor & 0xFFFF) == 0xFFFF)
            break;

        uint32 classRegister = pci_readConfigurationRegister(bus, slot, function, 0x08);
        uint8 class = (uint8)(classRegister >> 24);

        pci_currentEntry->bus = bus;
        pci_currentEntry->slot = slot;
        pci_currentEntry->function = function;
        pci_currentEntry->vendorID = deviceAndVendor & 0xFFFF;
        pci_currentEntry->deviceID = deviceAndVendor >> 16;
        pci_currentEntry->classID = class;

        // Make new entry.
        pci_currentEntry->next = memoryManager_allocate(sizeof(pciBus_Entry));
        pci_currentEntry = pci_currentEntry->next;
        pci_currentEntry->next = 0x0;
    }
}

uint32 pci_readConfigurationRegister(uint8 bus, uint8 slot, uint8 function, uint8 registerNo) {
    //Create the address of the register we want to read from. This is an address in "PCI configuration space", *not* I/O space or general memory space.
    uint32 address = 0x80000000 | (uint32)((bus & 0xFF) << 16) | (uint32)((slot & 0x1F) << 11) | (uint32)((function & 0x7) << 8) | (uint32)((registerNo & 0xFC));

    //Write the address of the register we want to the access request I/O port.
    portIO_write32(0xCF8, address);

    //Read the contents of the register at that address back from the data I/O port.
    return portIO_read32(0xCFC);
}
