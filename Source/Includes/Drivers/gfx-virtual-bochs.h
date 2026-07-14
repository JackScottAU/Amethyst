/**
 *  Amethyst Operating System - Driver for QEMU Standard Display Adapter.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
 * 
 *  This driver is also used by Bochs (where it originated).
 * 
 *  2D Capabilities:
 *  [X] 2D Framebuffer
 *  [-] Double Buffering
 *  [ ] Hardware Sprites (Cursor)
 *  [ ] Hardware Copy and Fill (Blitting)
*/

#include <Types.h>

#ifndef INCLUDES_QEMUVGA_H_
#define INCLUDES_QEMUVGA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <deviceTree.h>
#include <Graphics/canvas.h>
#include <Drivers/pciBus.h>

deviceTree_Entry* qemuVga_initialise(pciBus_Entry* pciDetails);

Canvas* qemuVga_getCanvas();

void qemuVga_setMode(uint16 width, uint16 height);


#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_QEMUVGA_H_
