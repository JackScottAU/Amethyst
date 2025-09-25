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


#define ATIRAGE128_REGOFFSET_CUR_OFFSET             0x260 / 4
#define ATIRAGE128_REGOFFSET_CUR_HORZ_VERT_POSN     0x264 / 4
#define ATIRAGE128_REGOFFSET_CUR_CLR0               0x26C / 4
#define ATIRAGE128_REGOFFSET_CUR_CLR1               0x270 / 4

/**
 * Register offset for CRTC_PITCH. Stores the number of bytes per display line.
 */
#define ATIRAGE128_REGOFFSET_CRTC_PITCH           0x022C / 4

typedef struct {
    uint16 hRes;
    uint16 vRes;

    uint16 hFront;
    uint16 hSync;
    uint16 hBack;

    /// @brief Horizontal Sync Polarity. 1 is positive.
    bool hSyncPolarity;

    uint16 vFront;
    uint16 vSync;
    uint16 vBack;

    /// @brief Vertical sync polarity. 1 is positive.
    bool vSyncPolarity;

    /// @brief Pixel clock in Hz.
    uint32 pixelClock;

    /// @brief Number of bits per pixel.
    uint8 depth;
} VideoMode;

const VideoMode videoMode1024x768 = {
    1024, 768, 
    24, 136, 160, 0,
    3, 6, 29, 0,
    65000000, 32
};

const VideoMode videoMode640x480 = {
    640, 480,
    16, 96, 48, 0,
    10, 2, 33, 0,
    25175000, 32
};


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

    // Pick our video mode.
    VideoMode mode = videoMode1024x768;

    uint32 hSyncStart = (mode.hRes + mode.hFront) / 8;
    uint32 hSyncWidth = mode.hSync / 8;
    uint32 hTotal = (mode.hRes + mode.hFront + mode.hSync + mode.hBack) / 8;
    uint32 hEnd = (mode.hRes / 8) -1;
    
    uint32 vSyncStart = (mode.vRes + mode.vFront);
    uint32 vSyncWidth = mode.vSync;
    uint32 vTotal = (mode.vRes + mode.vFront + mode.vSync + mode.vBack);
    uint32 vEnd = (mode.vRes) -1;

    uint8 hSyncPol = 1 - mode.hSyncPolarity;
    uint8 vSyncPol = 1 - mode.vSyncPolarity;

    uint32 depth = 0;

    switch(mode.depth) {
        case 32:
        depth = 6;
        break;

        default:
        depth = 6;
    }

    regs[ATIRAGE128_REGOFFSET_CRTC_H_TOTAL_DISP] = hTotal | (hEnd << 16);
    regs[ATIRAGE128_REGOFFSET_CRTC_V_TOTAL_DISP] = vTotal | (vEnd << 16);

    regs[ATIRAGE128_REGOFFSET_CRTC_H_SYNC_STRT_WID] = (hSyncStart << 3) | (hSyncWidth << 16) | (hSyncPol << 23);   // QEMU does not use this register at all.

    regs[ATIRAGE128_REGOFFSET_CRTC_V_SYNC_STRT_WID] = (vSyncStart) | (vSyncWidth << 16) | (vSyncPol << 23);        // QEMU deso not use this register at all.

    // Number of characters the screen is wide.
    regs[ATIRAGE128_REGOFFSET_CRTC_PITCH] = (mode.hRes) / 8;

    // Enable, 32bpp, extended.
    regs[ATIRAGE128_REGOFFSET_CRTC_GEN_CTRL] = (depth << 8) | (1 << 24) | (1 << 25) | (1 << 16); // 16 = cursors

    debug(LOGLEVEL_DEBUG, "ATI CRTC_GEN_CTRL ADDR: %h", &(regs[ATIRAGE128_REGOFFSET_CRTC_GEN_CTRL]));
    debug(LOGLEVEL_DEBUG, "ATI CRTC_GEN_CTRL: %h", regs[ATIRAGE128_REGOFFSET_CRTC_GEN_CTRL]);

    
    // fill cursor memnory with something, even if it makes no sense
    uint32* fbmem = (uint32*) 0xFFF00000; // skip 3meg
    for(int i = 0; i < 64 * 64; i++) {
        fbmem[i] = 0x8888CCCC + i;
    }

    
    regs[ATIRAGE128_REGOFFSET_CUR_OFFSET] = 0x300000; // 3mb
    regs[ATIRAGE128_REGOFFSET_CUR_HORZ_VERT_POSN] = 220 << 16 | 300; // pos
    regs[ATIRAGE128_REGOFFSET_CUR_CLR0] = 220 << 16 | 300 | 120 << 8;
    regs[ATIRAGE128_REGOFFSET_CUR_CLR1] = 120 << 16 | 200 | 80 << 8;


    atiRagePro_width  =mode.hRes;
    atiRagePro_height = mode.vRes;

    return device;
}

void atiRage128_dumpCursorPos() {
    uint32* regs = (uint32*) 0xFFBFC000;
    debug(LOGLEVEL_DEBUG, "ATI CUR_HORZ_VERT_POSN: %h", regs[ATIRAGE128_REGOFFSET_CUR_HORZ_VERT_POSN]);
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
