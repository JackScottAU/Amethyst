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
#include <Drivers/qemuVga.h>

uint32 qemuVga_width = 0;
uint32 qemuVga_height = 0;

deviceTree_Entry* qemu_device = NULL;

/**
 * ID register that tells us the version of the device.
 */
#define QEMUVGA_REGISTER_ID     0x0000
#define QEMUVGA_REGISTER_WIDTH  0x0001
#define QEMUVGA_REGISTER_HEIGHT 0x0002
#define QEMUVGA_REGISTER_DEPTH  0x0003
#define QEMUVGA_REGISTER_STATUS 0x0004

#define QEMUVGA_STATUS_DISABLED 0x0000
#define QEMUVGA_STATUS_ENABLED  0x0001
#define QEMUVGA_STATUS_LINEAR   0x0040

deviceTree_Entry* qemuVga_initialise(pciBus_Entry* pciDetails) {
    uint32 bus = pciDetails->bus;
    uint32 slot = pciDetails->slot;
    uint32 function = pciDetails->function;
    PageDirectory* pg = memoryManager_getCurrentPageDirectory();

    uint32 bar0 = pci_getBar(bus, slot, function, 0) & 0xFFFFFFF0;
    uint32 bar0size = pci_getBarSize(bus, slot, function, 0);
    // TODO(JackScottAU): check the size of the memory BAR and allocate more pages if needed.
    uint32 bar2 = pci_getBar(bus, slot, function, 2) & 0xFFFFFFFC;
    memoryManager_mapPhysicalMemoryPage(pg, (void*)0xFFC00000, (void*)bar0, 1024);
    memoryManager_mapPhysicalMemoryPage(pg, (void*)0xFFBFF000, (void*)bar2, 1);
    debug(LOGLEVEL_INFO, "VESA BAR0: %h", bar0);
    debug(LOGLEVEL_INFO, "VESA BAR2: %h", bar2);

    // BAR1: Reserved for 64-bit framebuffer addresses.
    // ROM:  Not needed.
    // IRQ:  None.

  //  memoryManager_printMemoryMap(pg);

    // can now read registers.
    uint16* bochsRegs = (uint16*) 0xFFBFF500;

    if (bochsRegs[QEMUVGA_REGISTER_ID] < 0xB0C2) {
        // Display adapter isn't good enough.
        // TODO(JackScottAU): unmap physdical memory pages we don't need.
        return NULL;
    }

    deviceTree_Entry* device = deviceTree_createDevice("QEMU Standard Display Adapter", DEVICETREE_TYPE_PCI, pciDetails);

    device->Resources = memoryManager_allocate(sizeof(DeviceResource) * 2);
    device->ResourceCount = 2;

    device->Resources[0].Type = DEVICE_RESOURCETYPE_MEM;
    device->Resources[0].StartAddress = bar0;
    device->Resources[0].Length = pci_getBarSize(bus, slot, function, 0);
    device->Resources[1].Type = DEVICE_RESOURCETYPE_MEM;
    device->Resources[1].StartAddress = bar2;
    device->Resources[1].Length = 0x1000;  // 4KiB

    deviceTree_Entry* monitor = deviceTree_createDevice("Generic Monitor", DEVICETREE_TYPE_OTHER, 0);

    deviceTree_addChild(device, monitor);

    qemu_device = device;

    return device;
}

Canvas* qemuVga_getCanvas() {
    // Check to make sure a mode has been set.
    if (qemuVga_width == 0 || qemuVga_height == 0) {
        debug(LOGLEVEL_ERROR, "No video mode has been set, cannot create a canvas.");
        return NULL;
    }

    Canvas* canvas = (Canvas*)memoryManager_allocate(sizeof(Canvas));
    canvas->framebuffer = (void*)0xFFC00000;
    canvas->height = qemuVga_height;
    canvas->width = qemuVga_width;

    return canvas;
}

void qemuVga_setMode(uint16 width, uint16 height) {
    uint16* bochsRegs = (uint16*) 0xFFBFF500;

    qemuVga_width = width;
    qemuVga_height = height;

    // BOCHS DISPLAY SETUP.
    bochsRegs[QEMUVGA_REGISTER_STATUS]  = QEMUVGA_STATUS_DISABLED;                          // disable.
    bochsRegs[QEMUVGA_REGISTER_WIDTH]   = width;
    bochsRegs[QEMUVGA_REGISTER_HEIGHT]  = height;
    bochsRegs[QEMUVGA_REGISTER_DEPTH]   = 0x20;
    bochsRegs[QEMUVGA_REGISTER_STATUS]  = QEMUVGA_STATUS_ENABLED | QEMUVGA_STATUS_LINEAR;   // enable, plus enable LFB.
}
