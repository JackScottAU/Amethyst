/**
 *  Amethyst Operating System - Driver for ATI Rage 128 Series Graphics Cards.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
 * 
 *  2D Capabilities:
 *  [X] 2D Framebuffer
 *  [-] Double Buffering
 *  [-] Hardware Sprites (Cursor)
 *  [-] Hardware Copy and Fill (Blitting)
*/

#include <memoryManager.h>
#include <debug.h>
#include <Drivers/pciBus.h>
#include <Drivers/atiRage128.h>
#include <stream.h>

VideoMode* atiRage128_currentMode = NULL;

deviceTree_Entry* atiRage128_device = NULL;


#define ATIRAGE128_REGOFFSET_CRTC_GEN_CTRL          0x0050

#define ATIRAGE128_REGOFFSET_CRTC_H_TOTAL_DISP      0x0200
#define ATIRAGE128_REGOFFSET_CRTC_H_SYNC_STRT_WID   0x0204
#define ATIRAGE128_REGOFFSET_CRTC_V_TOTAL_DISP      0x0208
#define ATIRAGE128_REGOFFSET_CRTC_V_SYNC_STRT_WID   0x020C

/// @brief Where in video memory the cursor can be found.
#define ATIRAGE128_REGOFFSET_CUR_OFFSET             0x260

#define ATIRAGE128_REGOFFSET_CUR_HORZ_VERT_POSN     0x264
#define ATIRAGE128_REGOFFSET_CUR_CLR0               0x26C
#define ATIRAGE128_REGOFFSET_CUR_CLR1               0x270

/**
 * Register offset for CRTC_PITCH. Stores the number of bytes per display line.
 */
#define ATIRAGE128_REGOFFSET_CRTC_PITCH           0x022C / 4

void atiRage128_writeRegister(uint32 registerOffset, uint32 data);
uint32 atiRage128_readRegister(uint32 registerOffset);

const VideoMode videoMode1024x768 = {
    "1024x768 @ 60Hz, 32bpp",
    1024, 768,
    24, 136, 160, 0,
    3, 6, 29, 0,
    65000000, 32
};

const VideoMode videoMode640x480 = {
    "640x480 @ 60Hz, 32bpp",
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
    memoryManager_mapPhysicalMemoryPage(pg, (void*)0xFFC00000, (void*)bar0, 1024);  // Framebuffer MM
    memoryManager_mapPhysicalMemoryPage(pg, (void*)0xFFBFC000, (void*)bar2, 4);     // Register MMIO
    debug(LOGLEVEL_INFO, "ATI RAGE 128 BAR0: %h", bar0);
    debug(LOGLEVEL_INFO, "ATI RAGE 128 BAR2: %h", bar2);

    // BAR0 : MEM_BASE
    // BAR1 : IO_BASE (mirror of first part of reg_base)
    // BAR2 : REG_BASE

    // can now read registers.
    volatile uint32* regs = (uint32*) 0xFFBFC000;


    deviceTree_Entry* device = deviceTree_createDevice("ATI Rage 128 Display Adapter", DEVICETREE_TYPE_PCI, pciDetails);

    uint32 irq = pci_readConfigurationRegister(pciDetails->bus, pciDetails->slot, pciDetails->function, 0x3C) & 0x000000FF;

    int resourceCount = 2;

    if (irq > 0) {
        resourceCount = 3;
    }

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

    atiRage128_device = device;

    // Pick our video mode.
    VideoMode* mode = &videoMode1024x768;

  //  atiRage128_setMode(mode);

/*
    // fill cursor memnory with something, even if it makes no sense
    uint32* fbmem = (uint32*) 0xFFF00000;   // skip 3meg
    for (int i = 0; i < 64 * 64; i++) {
        fbmem[i] = 0x8888CCCC + i;
    }

    regs[ATIRAGE128_REGOFFSET_CUR_OFFSET] = 0x300000;   // 3mb
    regs[ATIRAGE128_REGOFFSET_CUR_HORZ_VERT_POSN] = 220 << 16 | 300;    // pos
    regs[ATIRAGE128_REGOFFSET_CUR_CLR0] = 220 << 16 | 300 | 120 << 8;
    regs[ATIRAGE128_REGOFFSET_CUR_CLR1] = 120 << 16 | 200 | 80 << 8;*/

    return device;
}

void atiRage128_setMode(VideoMode* mode) {

    volatile uint32* regs = (uint32*) 0xFFBFC000;

    uint32 hSyncStart = (mode->hRes + mode->hFront) / 8;
    uint32 hSyncWidth = mode->hSync / 8;
    uint32 hTotal = (mode->hRes + mode->hFront + mode->hSync + mode->hBack) / 8;
    uint32 hEnd = (mode->hRes / 8) -1;

    uint32 vSyncStart = (mode->vRes + mode->vFront);
    uint32 vSyncWidth = mode->vSync;
    uint32 vTotal = (mode->vRes + mode->vFront + mode->vSync + mode->vBack);
    uint32 vEnd = (mode->vRes) -1;

    uint8 hSyncPol = 1 - mode->hSyncPolarity;
    uint8 vSyncPol = 1 - mode->vSyncPolarity;

    uint32 depth = 0;

    switch (mode->depth) {
        case 15:
            depth = 3;
            break;

        case 32:
            depth = 6;
            break;

        default:
            depth = 6;
    }

    atiRage128_writeRegister(ATIRAGE128_REGOFFSET_CRTC_H_TOTAL_DISP,    hTotal | (hEnd << 16));

    atiRage128_writeRegister(ATIRAGE128_REGOFFSET_CRTC_V_TOTAL_DISP,    vTotal | (vEnd << 16));

    atiRage128_writeRegister(ATIRAGE128_REGOFFSET_CRTC_H_SYNC_STRT_WID, (hSyncStart << 3) | (hSyncWidth << 16) | (hSyncPol << 23));   // QEMU does not use this register at all.

    atiRage128_writeRegister(ATIRAGE128_REGOFFSET_CRTC_V_SYNC_STRT_WID, (vSyncStart) | (vSyncWidth << 16) | (vSyncPol << 23));        // QEMU deso not use this register at all.

    // Number of characters the screen is wide.
    atiRage128_writeRegister(ATIRAGE128_REGOFFSET_CRTC_PITCH,           (mode->hRes) / 8);

    // Enable, 32bpp, extended.
    atiRage128_writeRegister(ATIRAGE128_REGOFFSET_CRTC_GEN_CTRL,        (depth << 8) | (1 << 24) | (1 << 25));

    // TODO(JackScottAU): pixel clock, memory clock

    atiRage128_currentMode = mode;
}

void atiRage128_writeRegister(uint32 registerOffset, uint32 data) {
    volatile uint32* regs = (uint32*) 0xFFBFC000;
    debug(LOGLEVEL_DEBUG, "ATIRAGE128: Writing data %h to register %h.", data, registerOffset);

    regs[registerOffset / 4] = data; // divide by 4 because a uint32 is 4 bytes.
}

uint32 atiRage128_readRegister(uint32 registerOffset) {
    volatile uint32* regs = (uint32*) 0xFFBFC000;

    uint32 data = regs[registerOffset / 4]; // divide by 4 because a uint32 is 4 bytes.
    
    debug(LOGLEVEL_DEBUG, "ATIRAGE128: Read data %h from register %h.", data, registerOffset);

    return data;
}

void atiRage128_dumpRegs(void (*putChar)(char)) {
    stream_printf(putChar, "ATI Dump Regs!\n");

    stream_printf(putChar, "ATIRAGE128_REGOFFSET_CRTC_GEN_CTRL[%h]: %H\n", ATIRAGE128_REGOFFSET_CRTC_GEN_CTRL, atiRage128_readRegister(ATIRAGE128_REGOFFSET_CRTC_GEN_CTRL));
    stream_printf(putChar, "ATIRAGE128_REGOFFSET_CRTC_PITCH[%h]: %H\n", ATIRAGE128_REGOFFSET_CRTC_PITCH, atiRage128_readRegister(ATIRAGE128_REGOFFSET_CRTC_PITCH));
    stream_printf(putChar, "ATIRAGE128_REGOFFSET_CRTC_H_TOTAL_DISP[%h]: %H\n", ATIRAGE128_REGOFFSET_CRTC_H_TOTAL_DISP, atiRage128_readRegister(ATIRAGE128_REGOFFSET_CRTC_H_TOTAL_DISP));
    stream_printf(putChar, "ATIRAGE128_REGOFFSET_CRTC_H_SYNC_STRT_WID[%h]: %H\n", ATIRAGE128_REGOFFSET_CRTC_H_SYNC_STRT_WID, atiRage128_readRegister(ATIRAGE128_REGOFFSET_CRTC_H_SYNC_STRT_WID));
    stream_printf(putChar, "ATIRAGE128_REGOFFSET_CRTC_V_TOTAL_DISP[%h]: %H\n", ATIRAGE128_REGOFFSET_CRTC_V_TOTAL_DISP, atiRage128_readRegister(ATIRAGE128_REGOFFSET_CRTC_V_TOTAL_DISP));
    stream_printf(putChar, "ATIRAGE128_REGOFFSET_CRTC_V_SYNC_STRT_WID[%h]: %H\n", ATIRAGE128_REGOFFSET_CRTC_V_SYNC_STRT_WID, atiRage128_readRegister(ATIRAGE128_REGOFFSET_CRTC_V_SYNC_STRT_WID));
}

void atiRage128_dumpCursorPos() {
    uint32* regs = (uint32*) 0xFFBFC000;
    debug(LOGLEVEL_DEBUG, "ATI CUR_HORZ_VERT_POSN: %h", regs[ATIRAGE128_REGOFFSET_CUR_HORZ_VERT_POSN]);
}

Canvas* atiRage128_getCanvas() {
    // Check to make sure a mode has been set.
    if (atiRage128_currentMode == NULL) {
        debug(LOGLEVEL_ERROR, "No video mode has been set, cannot create a canvas.");
        return NULL;
    }

    Canvas* canvas = (Canvas*)memoryManager_allocate(sizeof(Canvas));
    canvas->framebuffer = (void*)0xFFC00000;
    canvas->height = atiRage128_currentMode->vRes;
    canvas->width = atiRage128_currentMode->hRes;

    return canvas;
}
