/**
 *  Amethyst Operating System - PCI Bus Enumeration
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>
#include <portIO.h>
#include <vgaConsole.h>
#include <Drivers/pciBus.h>
#include <Drivers/piixide.h>
#include <stream.h>
#include <memoryManager.h>
#include <deviceTree.h>
#include "pci/deviceNames.h"
#include <debug.h>
#include <qemuVga.h>


#define PCIBUS_IOPORT_REQUEST   0x0CF8
#define PCIBUS_IOPORT_DATA      0x0CFC

#define PCIBUS_REGISTER_DEVICE_AND_VENDOR   0x00
#define PCIBUS_REGISTER_STATUS_AND_COMMAND  0x04
#define PCIBUS_REGISTER_CLASSCODES          0x08
#define PCIBUS_REGISTER_HEADERTYPE          0x0C
#define PCIBUS_REGISTER_ROMADDRESS          0x30

bool pci_isMultiFunctionDevice(uint8 bus, uint8 slot);

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

// There are 256 buses, each with up to 32 devices/slots, each with up to 8 functions.
#define PCI_BUSCOUNT 256
#define PCI_SLOTCOUNT 32
#define PCI_FUNCCOUNT  8

deviceTree_Entry* pciBus_initialise(void) {
    pci_enumerateBuses();

    return pci_addDevicesToTree();
}

deviceTree_Entry* pci_addDevicesToTree(void) {
    pci_currentEntry = pci_busEntries;

    // Assume the first device is the root hub.
    deviceTree_Entry* root = memoryManager_allocate(sizeof(deviceTree_Entry));
    root->next = NULL;
    root->child = NULL;
    root->name = pci_getNameFromVendorAndDevice(pci_currentEntry->vendorID, pci_currentEntry->deviceID);

    pci_currentEntry = pci_currentEntry->next;
    while (pci_currentEntry->next != 0x0) {

        if(pci_currentEntry->vendorID == 0x1234 && pci_currentEntry->deviceID == 0x1111) {
            deviceTree_Entry* device = qemuVga_initialise(pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function);

            deviceTree_addChild(root, device);
        } else if(pci_currentEntry->vendorID == 0x8086 && pci_currentEntry->deviceID == 0x7010) {
            // TODO: make this whole device detection thing better.
            deviceTree_Entry* device = piixide_initialise(pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function);

            deviceTree_addChild(root, device);
        } else {
            char* name = pci_getNameFromVendorAndDevice(pci_currentEntry->vendorID, pci_currentEntry->deviceID);

            deviceTree_Entry* device = deviceTree_createDevice(name, DEVICETREE_TYPE_PCI, pci_currentEntry);

            
            device->Resources = memoryManager_allocate(sizeof(DeviceResource) * 7); // brute force maximum for a pci device

            int i = 0;

            uint32 irq = pci_readConfigurationRegister(pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function, 0x3C) & 0x000000FF;
            if(irq > 0) {
                device->Resources[i].Type = DEVICE_RESOURCETYPE_IRQ;
                device->Resources[i].Flags = irq;

                i++;
            }

            for(int bar = 0; bar < 6; bar++) {
                uint32 val = pci_getBar(pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function, bar);

                if(val > 0) {
                    // we have a device.
                    device->Resources[i].Type = DEVICE_RESOURCETYPE_MEM;
                    if(val & 0x01) {
                        device->Resources[i].Type = DEVICE_RESOURCETYPE_IO;
                        val--;
                    }

                    device->Resources[i].StartAddress = val;
                    device->Resources[i].Length = pci_getBarSize(pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function, bar);

                    i++;
                }
            }

            
            device->ResourceCount = i;

            deviceTree_addChild(root, device);
        }
        

        pci_currentEntry = pci_currentEntry->next;
    }

    return root;
}

void pci_printBuses(void (*putChar)(char)) {
    stream_printf(putChar, "  +-----+------+------+--------+--------+------------------------------------+\n");
    stream_printf(putChar, "  | BUS | SLOT | FUNC | VENDOR | DEVICE | CLASS DESCRIPTION                  |\n");
    stream_printf(putChar, "  +-----+------+------+--------+--------+------------------------------------+\n");

    // Iterate through the bus entries stored.
    pci_currentEntry = pci_busEntries;
    while (pci_currentEntry->next != NULL) {
        // Print entry.
        stream_printf(putChar, "  | %d   | %d    | %d    | %h | %h | ",
        pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function,
        pci_currentEntry->vendorID, pci_currentEntry->deviceID);
        stream_printf(putChar, classNames[pci_currentEntry->classID]);
        stream_printf(putChar, " |\n");

        pci_currentEntry = pci_currentEntry->next;
    }

    stream_printf(putChar, "  +-----+------+------+--------+--------+------------------------------------+\n");
}

void pci_printBars(void (*putChar)(char)) {
    pci_currentEntry = pci_busEntries;
    while (pci_currentEntry->next != NULL) {
    char* name = pci_getNameFromVendorAndDevice(pci_currentEntry->vendorID, pci_currentEntry->deviceID);

        stream_printf(putChar, "[%d:%d:%d] %s:\n", pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function, name);

        uint32 classRegister = pci_readConfigurationRegister(pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function, PCIBUS_REGISTER_CLASSCODES);
        uint8 subclassID = (uint8)(classRegister >> 16 & 0xFF);
        uint8 progInterfaceID = (uint8)(classRegister >> 8 & 0xFF);
        uint8 revisionID = (uint8)(classRegister >> 0 & 0xFF);
        
        stream_printf(putChar, "  Class <%d:%d:%d.%d>: %s\n", pci_currentEntry->classID, subclassID, progInterfaceID, revisionID, classNames[pci_currentEntry->classID]);

        for(int bar = 0; bar < 6; bar++) {
            uint32 val = pci_getBar(pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function, bar);

            char* type = "MEM";
            bool prefetchable = false;

            if(val & 0x1) {
                type = "I/O";
                val--;
            }

            if(val & 0x8) {
                prefetchable = true;
                val -= 8;
            }

            if(val > 0) {
                uint32 size = pci_getBarSize(pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function, bar);

                stream_printf(putChar, "  BAR%d (%s): %h (size %h)", bar, type,val, size);

                if(prefetchable) {
                    stream_printf(putChar, " - Prefetchable");
                }

                stream_printf(putChar, "\n");
            }

        }

        uint32 rom = pci_readConfigurationRegister(pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function, PCIBUS_REGISTER_ROMADDRESS);
        if(rom > 0) {
            stream_printf(putChar, "  ROM:  %h\n", rom);
        }

        uint32 irq = pci_readConfigurationRegister(pci_currentEntry->bus, pci_currentEntry->slot, pci_currentEntry->function, 0x3C);
        if((irq & 0x000000FF) > 0) {
            stream_printf(putChar, "  IRQ:  %d\n", irq & 0x000000FF);
        }

        pci_currentEntry = pci_currentEntry->next;
    }
}

uint32 pci_getBar(uint8 bus, uint8 slot, uint8 function, uint8 bar) {
    uint32 registerNo = 0x10 + (bar * 4);

    debug(LOGLEVEL_DEBUG, "Register Number: %h", registerNo);

    return pci_readConfigurationRegister(bus, slot, function, registerNo);
}


uint32 pci_getBarSize(uint8 bus, uint8 slot, uint8 function, uint8 bar) {
    uint32 registerNo = 0x10 + (bar * 4);

    debug(LOGLEVEL_DEBUG, "Register Number: %h", registerNo);

    uint32 address = pci_readConfigurationRegister(bus, slot, function, registerNo);

    pci_writeConfigurationRegister(bus, slot, function, registerNo, 0xFFFFFFFF);

    uint32 size = pci_readConfigurationRegister(bus, slot, function, registerNo);

    size = size & 0xFFFFFFF0;

    size = ~size;

    size = size + 1;

    pci_writeConfigurationRegister(bus, slot, function, registerNo, address);

    return size;
}

void pci_enumerateBuses(void) {
    uint16 bus;

    // very brute force. need to fix this.

    // Init the table.
    pci_busEntries = memoryManager_allocate(sizeof(pciBus_Entry));
    pci_currentEntry = pci_busEntries;
    pci_currentEntry->next = 0x0;   // set next to zero. this is added later.

    for (bus = 0; bus < PCI_BUSCOUNT; bus++) {
        pci_checkBus(bus);
    }
}

void pci_checkBus(uint8 bus) {
    uint8 slot;

    for (slot = 0; slot < PCI_SLOTCOUNT; slot++)
        pci_checkSlot(bus, slot);
}

bool pci_isMultiFunctionDevice(uint8 bus, uint8 slot) {
    uint32 header = pci_readConfigurationRegister(bus, slot, 0, PCIBUS_REGISTER_HEADERTYPE);

    return header & 0x00800000;
}

void pci_checkSlot(uint8 bus, uint8 slot) {
    // Check whether we have a multi-function device and need to inspect all functions at this slot.
    uint8 numberOfFunctions = pci_isMultiFunctionDevice(bus, slot) ? PCI_FUNCCOUNT: 1;

    for (uint8 function = 0; function < numberOfFunctions; function++) {
        uint32 deviceAndVendor = pci_readConfigurationRegister(bus, slot, function, PCIBUS_REGISTER_DEVICE_AND_VENDOR);

        if ((deviceAndVendor & 0xFFFF) == 0xFFFF)
            break;

        uint32 classRegister = pci_readConfigurationRegister(bus, slot, function, PCIBUS_REGISTER_CLASSCODES);
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

/**
 * Calculates the address in the PCI configuration space for a given PCI bus:slot:function and register number.
*/
uint32 pci_calculateRegisterAddress(uint8 bus, uint8 slot, uint8 function, uint8 registerNo) {
    // FORMAT: 0x80BBSFRR
    // Where BB is bus number, SF are slot and function bits, and RR is register number.

    // Create the address of the register we want to read from.
    // This is an address in "PCI configuration space", *not* I/O space or general memory space.
    return 0x80000000 | (uint32)((bus & 0xFF) << 16) | (uint32)((slot & 0x1F) << 11) |
    (uint32)((function & 0x7) << 8) | (uint32)((registerNo & 0xFC));
}

uint32 pci_readConfigurationRegister(uint8 bus, uint8 slot, uint8 function, uint8 registerNo) {
    // Write the address of the register we want to the access request I/O port.
    portIO_write32(PCIBUS_IOPORT_REQUEST, pci_calculateRegisterAddress(bus, slot, function, registerNo));

    // Read the contents of the register at that address back from the data I/O port.
    return portIO_read32(PCIBUS_IOPORT_DATA);
}

void pci_writeConfigurationRegister(uint8 bus, uint8 slot, uint8 function, uint8 registerNo, uint32 data) {
    // Write the address of the register we want to the access request I/O port.
    portIO_write32(PCIBUS_IOPORT_REQUEST, pci_calculateRegisterAddress(bus, slot, function, registerNo));

    portIO_write32(PCIBUS_IOPORT_DATA, data);
}
