/* 
 * File:   pciBus.h
 * Author: jscott
 *
 * Created on 25 January 2013, 12:03 PM
 */

#ifndef __PCIBUS_H
#define	__PCIBUS_H

#ifdef	__cplusplus
extern "C" {
#endif

	#include <Types.h>
	#include <deviceTree.h>

	void pci_enumerateBuses(void);
	void pci_printBuses(void);
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

	typedef struct pciBus_Entry_s {
		uint8 bus;
		uint8 slot;
		uint8 function;
		uint16 vendorID;
		uint16 deviceID;
		uint8 classID;
		struct pciBus_Entry_s* next;
	} pciBus_Entry;

#ifdef	__cplusplus
}
#endif

#endif	/* __PCIBUS_H */

