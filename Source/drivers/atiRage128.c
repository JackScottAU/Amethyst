// BAR0 : MEM_BASE
// BAR1 : IO_BASE (mirror of first part of reg_base)
// BAR2 : REG_BASE

/**
 *  Amethyst Operating System - Driver for QEMU Standard Display Adapter.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
 * 
 *  This driver is also used by Bochs (where it originated).
 * 
 *  Capabilities:
 *  [X] 2D Framebuffer (getCanvas())
 *  [ ] Anything else
*/

#include <memoryManager.h>
#include <debug.h>
#include <Drivers/pciBus.h>
#include <Drivers/atiRage128.h>

uint32 atiRagePro_width = 0;
uint32 atiRagePro_height = 0;

deviceTree_Entry* atiRagePro_device = NULL;


#define ATIRAGE128_REGOFFSET_CRTC_GEN_CTRL        0x0050 / 4

#define ATIRAGE128_REGOFFSET_CRTC_H_TOTAL_DISP    0x0200 / 4
#define ATIRAGE128_REGOFFSET_CRTC_H_SYNC_STRT_WID 0x0204 / 4
#define ATIRAGE128_REGOFFSET_CRTC_V_TOTAL_DISP    0x0208 / 4
#define ATIRAGE128_REGOFFSET_CRTC_V_SYNC_STRT_WID 0x020C / 4

/**
 * Register offset for CRTC_PITCH. Stores the number of bytes per display line.
 */
#define ATIRAGE128_REGOFFSET_CRTC_PITCH           0x022C / 4


deviceTree_Entry* atiRage128_initialise(pciBus_Entry* pciDetails) {
    uint32 bus = pciDetails->bus;
    uint32 slot = pciDetails->slot;
    uint32 function = pciDetails->function;
    PageDirectory* pg = memoryManager_getCurrentPageDirectory();

    uint32 bar0 = pci_getBar(bus, slot, function, 0) & 0xFFFFFFF0;
    uint32 bar0size = pci_getBarSize(bus, slot, function, 0);
    // TODO(JackScottAU): check the size of the memory BAR and allocate more pages if needed.
    uint32 bar2 = pci_getBar(bus, slot, function, 2) & 0xFFFFFFFC;
    memoryManager_mapPhysicalMemoryPage(pg, (void*)0xFFC00000, (void*)bar0, 1024); // Framebuffer MM
    memoryManager_mapPhysicalMemoryPage(pg, (void*)0xFFBFC000, (void*)bar2, 4); // Register MMIO
    debug(LOGLEVEL_INFO, "ATI RAGE 128 BAR0: %h", bar0);
    debug(LOGLEVEL_INFO, "ATI RAGE 128 BAR2: %h", bar2);

    // BAR1: Reserved for 64-bit framebuffer addresses.
    // ROM:  Not needed.
    // IRQ:  None.

  //  memoryManager_printMemoryMap(pg);

    // can now read registers.
    uint32* regs = (uint32*) 0xFFBFC000;


    deviceTree_Entry* device = deviceTree_createDevice("ATI Rage 128 Display Adapter", DEVICETREE_TYPE_PCI, pciDetails);

    uint32 irq = pci_readConfigurationRegister(pciDetails->bus, pciDetails->slot, pciDetails->function, 0x3C) & 0x000000FF;

    int resourceCount = 2;

    if(irq > 0) {resourceCount = 3;}

    device->Resources = memoryManager_allocate(sizeof(DeviceResource) * resourceCount);
    device->ResourceCount = resourceCount;

    device->Resources[0].Type = DEVICE_RESOURCETYPE_MEM;
    device->Resources[0].StartAddress = bar0;
    device->Resources[0].Length = pci_getBarSize(bus, slot, function, 0);
    device->Resources[1].Type = DEVICE_RESOURCETYPE_MEM;
    device->Resources[1].StartAddress = bar2;
    device->Resources[1].Length = 0x4000;  // 16KiB

    if (irq > 0) {
        device->Resources[2].Type = DEVICE_RESOURCETYPE_IRQ;
        device->Resources[2].Flags = irq;
    }

    deviceTree_Entry* monitor = deviceTree_createDevice("Generic Monitor", DEVICETREE_TYPE_OTHER, 0);

    deviceTree_addChild(device, monitor);

    atiRagePro_device = device;

    // 1024x768
    regs[ATIRAGE128_REGOFFSET_CRTC_H_TOTAL_DISP] = 0xA8 | (0x7F << 16);

    regs[ATIRAGE128_REGOFFSET_CRTC_H_SYNC_STRT_WID] = (0x83 << 3) | (0x17 << 16) | (1 << 23);

    regs[ATIRAGE128_REGOFFSET_CRTC_V_TOTAL_DISP] = 0x326 | (0x2FF << 16);
    regs[ATIRAGE128_REGOFFSET_CRTC_V_SYNC_STRT_WID] = (0x302) | (0x6 << 16) | (1 << 23);

    // Number of bytes we need per line.
    regs[ATIRAGE128_REGOFFSET_CRTC_PITCH] = 1024*4;

    // Enable, 32bpp, extended.
    regs[ATIRAGE128_REGOFFSET_CRTC_GEN_CTRL] = (6 << 8) | (1 << 24) | (1 << 25);

    debug(LOGLEVEL_DEBUG, "ATI CRTC_GEN_CTRL ADDR: %h", &(regs[ATIRAGE128_REGOFFSET_CRTC_GEN_CTRL]));
    debug(LOGLEVEL_DEBUG, "ATI CRTC_GEN_CTRL: %h", regs[ATIRAGE128_REGOFFSET_CRTC_GEN_CTRL]);

    

    atiRagePro_width  =1024;
    atiRagePro_height = 768;

    return device;
}

Canvas* atiRage128_getCanvas() {
    // Check to make sure a mode has been set.
    if (atiRagePro_width == 0 || atiRagePro_height == 0) {
        debug(LOGLEVEL_ERROR, "No video mode has been set, cannot create a canvas.");
        return NULL;
    }

    Canvas* canvas = (Canvas*)memoryManager_allocate(sizeof(Canvas));
    canvas->framebuffer = (void*)0xFFC00000;
    canvas->height = atiRagePro_height;
    canvas->width = atiRagePro_width;

    return canvas;
}
