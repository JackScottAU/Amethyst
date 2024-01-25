/**
 * Enumerates PCI buses. Will do more eventually.
 * The reference for most of the code within this file is http://wiki.osdev.org/PCI
 */

#include <Types.h>
#include <portIO.h>
#include <vgaConsole.h>
#include <pciBus.h>

const char* classNames[] = {
	"Unknown",
	"Mass Storage Controller",
	"Network Controller",
	"Display Controller",
	"Multimedia Controller",
	"Memory Controller",
	"Bridge Device",
	"Simple Communications Controller",
	"Base System Peripherals",
	"Input Devices",
	"Docking Stations",
	"Processors",
	"Serial Bus Controllers",
	"Wireless Controllers",
	"Intelligent I/O Controllers",
	"Satellite Communication Controllers",
	"Encryption/Decryption Controllers",
	"Data Acquisition and Signal Processing Controllers",
	"Reserved"
};

void pci_enumerateBuses(void)
{
	uint16 bus;
	
	//very brute force. need to fix this.
	
	for(bus = 0; bus <= 255; bus++)
		pci_checkBus(bus);
}

void pci_checkBus(uint8 bus)
{
	uint8 slot;
	
	for(slot = 0; slot < 32; slot++)
		pci_checkSlot(bus, slot);
}

void pci_checkSlot(uint8 bus, uint8 slot)
{
	uint8 function = 0;
	
	for(function = 0; function < 32; function++)
	{
		uint32 deviceAndVendor = pci_readConfigurationRegister(bus, slot, function, 0x00);
		
		if((deviceAndVendor & 0xFFFF) == 0xFFFF)
			break;

        uint32 classRegister = pci_readConfigurationRegister(bus, slot, function, 0x08);
		uint8 class = (uint8) (classRegister >> 24);
		
		vgaConsole_printf("PCI device at %d:%d:%d has vendor %h and device %h, of class ",bus,slot,function,deviceAndVendor&0xFFFF,deviceAndVendor>>16);
        vgaConsole_printf(classNames[class]);
		
		vgaConsole_printf(".\n");
		
	}
}

uint32 pci_readConfigurationRegister(uint8 bus, uint8 slot, uint8 function, uint8 registerNo)
{
	//Create the address of the register we want to read from. This is an address in "PCI configuration space", *not* I/O space or general memory space.
	uint32 address = 0x80000000 | (uint32)((bus & 0xFF)<<16) | (uint32)((slot & 0x1F)<<11) | (uint32)((function & 0x5)<<8) | (uint32)((registerNo & 0xFC));
	
	//Write the address of the register we want to the access request I/O port.
	portIO_write32(0xCF8, address);
	
	//Read the contents of the register at that address back from the data I/O port.
	return portIO_read32(0xCFC);
}
