#include <Drivers/piixide.h>
#include <Drivers/pciBus.h>
#include <memoryManager.h>

deviceTree_Entry* piixide_initialise(pciBus_Entry* pciDetails) {
    uint32 bus = pciDetails->bus;
    uint32 slot = pciDetails->slot;
    uint32 function = pciDetails->function;

    deviceTree_Entry* device = deviceTree_createDevice("Generic PCI IDE Controller", DEVICETREE_TYPE_PCI, pciDetails);

    // check programming interface byte: if bits set then we use BARs, otherwise use default addresses.
    uint32 classLong = pci_readConfigurationRegister(bus, slot, function, 0x08);
    uint8 programmingInterface = (classLong > 8) & 0xFF;

    // check programming interface byte:
    if(programmingInterface & 2) {
        // todo
    }

    // I/O bar for bus mastering.
    uint32 bar4 = pci_getBar(bus, slot, function, 4) & 0xFFFFFFFC;
    uint32 bar4size = pci_getBarSize(bus, slot, function, 4);

    device->ResourceCount = 0;

    deviceTree_Entry* channel1 = deviceTree_createDevice("IDE Channel", DEVICETREE_TYPE_OTHER, NULL);
    deviceTree_addChild(device, channel1);
    channel1->ResourceCount = 4;
    channel1->Resources = memoryManager_allocate(sizeof(DeviceResource) * 4);
    channel1->Resources[0].Type = DEVICE_RESOURCETYPE_IO;
    channel1->Resources[0].StartAddress = 0x1F0;
    channel1->Resources[0].Length = 8;
    
    channel1->Resources[1].Type = DEVICE_RESOURCETYPE_IO;
    channel1->Resources[1].StartAddress = 0x3F6;
    channel1->Resources[1].Length = 2;

    channel1->Resources[2].Type = DEVICE_RESOURCETYPE_IO;
    channel1->Resources[2].StartAddress = bar4;
    channel1->Resources[2].Length = bar4size / 2;

    channel1->Resources[3].Type = DEVICE_RESOURCETYPE_IRQ;
    channel1->Resources[3].Flags = 14;

    deviceTree_Entry* channel2 = deviceTree_createDevice("IDE Channel", DEVICETREE_TYPE_OTHER, NULL);
    deviceTree_addChild(device, channel2);
    channel2->ResourceCount = 4;
    channel2->Resources = memoryManager_allocate(sizeof(DeviceResource) * 4);
    channel2->Resources[0].Type = DEVICE_RESOURCETYPE_IO;
    channel2->Resources[0].StartAddress = 0x170;
    channel2->Resources[0].Length = 8;
    
    channel2->Resources[1].Type = DEVICE_RESOURCETYPE_IO;
    channel2->Resources[1].StartAddress = 0x376;
    channel2->Resources[1].Length = 2;

    channel2->Resources[2].Type = DEVICE_RESOURCETYPE_IO;
    channel2->Resources[2].StartAddress = bar4 + 8;
    channel2->Resources[2].Length = bar4size / 2;

    channel2->Resources[3].Type = DEVICE_RESOURCETYPE_IRQ;
    channel2->Resources[3].Flags = 15;

 //   (ports 0x1F0-0x1F7, 0x3F6-0x3F7, IRQ14).
 //   (ports 0x170-0x177, 0x376-0x377, IRQ15).
/*
 channels[ATA_PRIMARY  ].base  = (BAR0 & 0xFFFFFFFC) + 0x1F0 * (!BAR0);
   channels[ATA_PRIMARY  ].ctrl  = (BAR1 & 0xFFFFFFFC) + 0x3F6 * (!BAR1);
   channels[ATA_SECONDARY].base  = (BAR2 & 0xFFFFFFFC) + 0x170 * (!BAR2);
   channels[ATA_SECONDARY].ctrl  = (BAR3 & 0xFFFFFFFC) + 0x376 * (!BAR3);
   channels[ATA_PRIMARY  ].bmide = (BAR4 & 0xFFFFFFFC) + 0; // Bus Master IDE
   channels[ATA_SECONDARY].bmide = (BAR4 & 0xFFFFFFFC) + 8; // Bus Master IDE*/

   return device;
}
