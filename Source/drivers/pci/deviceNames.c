#include "deviceNames.h"

struct pci_deviceName {
    uint16 vendor;
    uint16 device;
    char* name;
};

const struct pci_deviceName deviceNames[] = {
    { 0x8086, 0x100E, "82540EM Gigabit Ethernet Controller" },
    { 0x8086, 0x1237, "i440FX Northbridge & PCI Root Hub" },
    { 0x8086, 0x7000, "PIIX3 PCI-to-ISA Bridge" },
    { 0x8086, 0x7010, "PIIX3 IDE Controller" },
    { 0x8086, 0x7020, "PIIX3 UHCI USB Controller" },
    { 0x8086, 0x7113, "PIIX4 ACPI" },
    { 0x1234, 0x1111, "QEMU Standard VGA Graphics Adapter" },

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
