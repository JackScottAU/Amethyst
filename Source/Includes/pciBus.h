/**
 *  Amethyst Operating System - PCI bus enumeration.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_PCIBUS_H_
#define INCLUDES_PCIBUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Types.h>
#include <deviceTree.h>

void pci_enumerateBuses(void);
void pci_printBuses(void (*putChar)(char));
void pci_checkBus(uint8 bus);
void pci_checkSlot(uint8 bus, uint8 slot);
deviceTree_Entry* pci_addDevicesToTree(void);
void pci_printBars(void (*putChar)(char));

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

typedef struct pciBus_Entry_s {
    uint8 bus;
    uint8 slot;
    uint8 function;
    uint16 vendorID;
    uint16 deviceID;
    uint8 classID;
    struct pciBus_Entry_s* next;
} pciBus_Entry;

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_PCIBUS_H_
