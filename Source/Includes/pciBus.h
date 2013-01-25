/* 
 * File:   pciBus.h
 * Author: jscott
 *
 * Created on 25 January 2013, 12:03 PM
 */

#ifndef PCIBUS_H
#define	PCIBUS_H

#include <Types.h>

void pci_enumerateBuses(void);
void pci_checkBus(uint8 bus);
void pci_checkSlot(uint8 bus, uint8 slot);
uint32 pci_readConfigurationRegister(uint8 bus, uint8 slot, uint8 function, uint8 registerNo);

#endif	/* PCIBUS_H */

