/**
 *  Amethyst Operating System - List of PCI device names.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include "deviceNames.h"

struct pci_deviceName {
    uint16 vendor;
    uint16 device;
    char* name;
};

const struct pci_deviceName deviceNames[] = {
    { 0x8086, 0x100E, "82540EM Gigabit Ethernet Controller" },
    { 0x8086, 0x1237, "i440FX PCI Host Bridge" },

    { 0x8086, 0x7000, "PIIX3 PCI-to-ISA Bridge" },
    { 0x8086, 0x7010, "PIIX3 IDE Controller" },
    { 0x8086, 0x7020, "PIIX3 UHCI USB Controller" },

    { 0x8086, 0x7110, "PIIX4 PCI-to-ISA Bridge" },
    { 0x8086, 0x7111, "PIIX4 IDE Controller" },
    { 0x8086, 0x7112, "PIIX4 UHCI USB Controller" },
    { 0x8086, 0x7113, "PIIX4 ACPI" },

    { 0x8086, 0x7192, "i440BX/ZX/DX PCI Host Bridge" },

    { 0x10EC, 0x8139, "Realtek 8139 Network Adapter" },

    { 0x1234, 0x1111, "QEMU Standard VGA Graphics Adapter" },
    { 0x1B36, 0x0011, "QEMU PVPANIC Device"},

    { 0x1414, 0x5353, "Microsoft Hyper-V Graphics Adapter" },

    // The last entry in the table is a dummy entry to mark the end of the table.
    { 0xFFFF, 0xFFFF, "DUMMY ENTRY - END OF TABLE"}
};

char* pci_getNameFromVendorAndDevice(uint16 vendor, uint16 device) {
    struct pci_deviceName* deviceNameEntry = deviceNames;

    while (deviceNameEntry->vendor != 0xFFFF && deviceNameEntry->device != 0xFFFF) {
        if (deviceNameEntry->vendor == vendor && deviceNameEntry->device == device) {
            return deviceNameEntry->name;
        }

        deviceNameEntry++;
    }

    return "Unknown PCI Device";
}
