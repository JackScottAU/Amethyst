/**
 Enumerates PCI buses. Will do more eventually.
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
	uint16 bus = 0;
	
	//very brute force. need to fix this.
	
	//for(bus = 0; bus <= 255; bus++)
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
		uint32 manuslot = pci_readConfigurationRegister(bus, slot, function, 0);
		
		if((manuslot & 0xFFFF) == 0xFFFF)
			break;
		
		vgaConsole_printf("Found PCI device with vendor %h and device %h\n",manuslot&0xFFFF,manuslot>>16);
		//uint32 classRegister = pci_readConfigurationRegister(bus, slot, function, 0x08);
		//uint8 class = (uint8) classRegister >> 24;
		//vgaConsole_printf("it has class %h\n",classRegister);
		//vgaConsole_printf(classNames[class]);
	}
}

uint32 pci_readConfigurationRegister(uint8 bus, uint8 slot, uint8 function, uint8 registerNo)
{
	uint32 address;
	address = 0x80000000 | (uint32)((bus & 0xFF)<<16) | (uint32)((slot & 0x1F)<<11) | (uint32)((function & 0x5)<<8) | (uint32)((registerNo & 0x2F)<<2);
	
	//vgaConsole_printf("Checking PCI bus for: %h\n",address);
	
	writeLong(0xCF8, address);
	
	return readLong(0xCFC);
}