/**
 *  Amethyst Operating System - PCI Bus Driver.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
 * 
 *  This driver supports detection and enumeration of the PCI bus (including PCI Express, AGP and CardBus) and detection
 *  and initialisation of child devices (such as display adapters, network controllers, storage controllers, etc).
*/

#ifndef INCLUDES_DRIVERS_PCIBUS_H_
#define INCLUDES_DRIVERS_PCIBUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Types.h>
#include <deviceTree.h>

/**
 * Performs detection and enumeration of the PCI buses, and initialises any found child devices.
 */
deviceTree_Entry* pciBus_initialise(void);

void pci_enumerateBuses(void);
void pci_checkBus(uint8 bus);
void pci_checkSlot(uint8 bus, uint8 slot);
deviceTree_Entry* pci_addDevicesToTree(void);

/**
 * Reads the PCI configuration registers for a particular PCI device.
 * @param bus PCI bus number. 8 bits.
 * @param slot Which slot (device) on that PCI bus. 5 bits.
 * @param function Which function of that slot/device. 3 bits.
 * @param registerNo Which register to read. Must be a multiple of 4. 6 bits.
 * @return The contents of the specified register for that bus/slot/function.
 */
uint32 pci_readConfigurationRegister(uint8 bus, uint8 slot, uint8 function, uint8 registerNo);

void pci_writeConfigurationRegister(uint8 bus, uint8 slot, uint8 function, uint8 registerNo, uint32 data);
uint32 pci_calculateRegisterAddress(uint8 bus, uint8 slot, uint8 function, uint8 registerNo);
uint32 pci_getBar(uint8 bus, uint8 slot, uint8 function, uint8 bar);
uint32 pci_getBarSize(uint8 bus, uint8 slot, uint8 function, uint8 bar);

typedef struct pciBus_Entry_s {
    uint8 bus;
    uint8 slot;
    uint8 function;
    uint16 vendorID;
    uint16 deviceID;
    uint8 classID;
    uint8 subClassID;
    struct pciBus_Entry_s* next;
} pciBus_Entry;

void pciBus_printDeviceInformation(void (*putChar)(char), pciBus_Entry* device, uint32 depth);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_DRIVERS_PCIBUS_H_
