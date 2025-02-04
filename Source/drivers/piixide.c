#include <Drivers/piixide.h>
#include <Drivers/pciBus.h>
#include <memoryManager.h>
#include <portIO.h>
#include <debug.h>


deviceTree_Entry* piixide_decodeDriveSignature(uint32 cl, uint32 ch) {
    if (cl==0x14 && ch==0xEB) {
        return deviceTree_createDevice("PATA CD-ROM Drive?!", DEVICETREE_TYPE_OTHER, NULL);
    }
    
	if (cl==0x69 && ch==0x96) {
        return deviceTree_createDevice("SATA CD-ROM Drive?!", DEVICETREE_TYPE_OTHER, NULL);
    }

	if (cl==0 && ch == 0) {
        return deviceTree_createDevice("PATA HDD Drive?!", DEVICETREE_TYPE_OTHER, NULL);
    }

	if (cl==0x3c && ch==0xc3) {
        return deviceTree_createDevice("SATA HDD Drive?!", DEVICETREE_TYPE_OTHER, NULL);
    }

	return NULL;
}

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

   /* on Primary bus: ctrl->base =0x1F0, ctrl->dev_ctl =0x3F6. REG_CYL_LO=4, REG_CYL_HI=5, REG_DEVSEL=6 */

    // reset
    portIO_write8(0x3F6, 4);
	portIO_read8(0x3F6);			/* wait 400ns for drive select to work */
	portIO_read8(0x3F6);
	portIO_read8(0x3F6);
	portIO_read8(0x3F6);
    portIO_write8(0x3F6, 0);
	
    /* waits until master drive is ready again */
	portIO_write8(0x1F0 + 6, 0xA0 | 0<<4);
	portIO_read8(0x3F6);			/* wait 400ns for drive select to work */
	portIO_read8(0x3F6);
	portIO_read8(0x3F6);
	portIO_read8(0x3F6);

	unsigned cl=portIO_read8(0x1F0 + 4);	/* get the "signature bytes" */
	unsigned ch=portIO_read8(0x1F0 + 5);

    deviceTree_addChild(channel1, piixide_decodeDriveSignature(cl, ch));

    debug(LOGLEVEL_DEBUG, "ATA PM: %h %h", cl, ch);

    /* waits until master drive is ready again */
	portIO_write8(0x1F0 + 6, 0xA0 | 1<<4);
	portIO_read8(0x3F6);			/* wait 400ns for drive select to work */
	portIO_read8(0x3F6);
	portIO_read8(0x3F6);
	portIO_read8(0x3F6);

	cl=portIO_read8(0x1F0 + 4);	/* get the "signature bytes" */
	ch=portIO_read8(0x1F0 + 5);

    debug(LOGLEVEL_DEBUG, "ATA PS: %h %h", cl, ch);

    deviceTree_addChild(channel1, piixide_decodeDriveSignature(cl, ch));

    // reset
    portIO_write8(0x376, 4);
	portIO_read8(0x376);			/* wait 400ns for drive select to work */
	portIO_read8(0x376);
	portIO_read8(0x376);
	portIO_read8(0x376);
    portIO_write8(0x376, 0);
	
    /* waits until master drive is ready again */
	portIO_write8(0x170 + 6, 0xA0 | 0<<4);
	portIO_read8(0x376);			/* wait 400ns for drive select to work */
	portIO_read8(0x376);
	portIO_read8(0x376);
	portIO_read8(0x376);

	cl=portIO_read8(0x170 + 4);	/* get the "signature bytes" */
	ch=portIO_read8(0x170 + 5);

    debug(LOGLEVEL_DEBUG, "ATA SM: %h %h", cl, ch);

    deviceTree_addChild(channel2, piixide_decodeDriveSignature(cl, ch));

    /* waits until master drive is ready again */
	portIO_write8(0x170 + 6, 0xA0 | 1<<4);
	portIO_read8(0x376);			/* wait 400ns for drive select to work */
	portIO_read8(0x376);
	portIO_read8(0x376);
	portIO_read8(0x376);

	cl=portIO_read8(0x170 + 4);	/* get the "signature bytes" */
	ch=portIO_read8(0x170 + 5);

    debug(LOGLEVEL_DEBUG, "ATA SS: %h %h", cl, ch);

    deviceTree_addChild(channel2, piixide_decodeDriveSignature(cl, ch));


	/* differentiate ATA, ATAPI, SATA and SATAPI 
	*/


   return device;
}
