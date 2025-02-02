#include <qemuVga.h>
#include <memoryManager.h>
#include <debug.h>
#include <pciBus.h>

uint32 qemuVga_width = 0;
uint32 qemuVga_height = 0;

deviceTree_Entry* qemu_device = NULL;

deviceTree_Entry* qemuVga_initialise(uint32 bus, uint32 slot, uint32 function)
{

    PageDirectory* pg = memoryManager_getCurrentPageDirectory();

    uint32 bar0 = pci_getBar(bus, slot, function, 0) & 0xFFFFFFF0;
    // TODO: check the size of the memory BAR and allocate more pages if needed.
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

    deviceTree_Entry* device = deviceTree_createDevice("QEMU Standard Display Adapter", DEVICETREE_TYPE_PCI, 0);

    qemu_device = device;

    return device;
}

Canvas* qemuVga_getCanvas() {
    // Check to make sure a mode has been set.
    if(qemuVga_width == 0 || qemuVga_height == 0) {
        debug(LOGLEVEL_ERROR, "No video mode has been set, cannot create a canvas.");
        return NULL;
    }

    Canvas* canvas = (Canvas*)memoryManager_allocate(sizeof(Canvas));
    canvas->framebuffer = (void*)0xFFC00000;
    canvas->height =qemuVga_height;
    canvas->width = qemuVga_width;

    return canvas;
}

void qemuVga_setMode(uint16 width, uint16 height) {
uint16* bochsRegs = (uint16*) 0xFFBFF500;

    qemuVga_width = width;
    qemuVga_height = height;

    // BOCHS DISPLAY SETUP.
    bochsRegs[4] = 0; // disable.
    bochsRegs[1] = width;
    bochsRegs[2] = height;
    bochsRegs[3] = 0x20;
    bochsRegs[4] = 0x1 | 0x40; // enable, plus enable LFB.
}
