/**
 *  Amethyst Operating System - List of PCI device names.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef DRIVERS_PCI_DEVICENAMES_H_
#define DRIVERS_PCI_DEVICENAMES_H_

#include <Types.h>



char* pci_getNameFromVendorAndDevice(uint16 vendor, uint16 device);

#endif  // DRIVERS_PCI_DEVICENAMES_H_
