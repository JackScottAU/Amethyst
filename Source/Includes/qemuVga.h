/**
 *  Amethyst Operating System - Definitions for kernel device driver tree.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>

#ifndef INCLUDES_QEMUVGA_H_
#define INCLUDES_QEMUVGA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <deviceTree.h>
#include <Graphics/canvas.h>

deviceTree_Entry* qemuVga_initialise(uint32 bus, uint32 slot, uint32 function);

Canvas* qemuVga_getCanvas();

void qemuVga_setMode(uint16 width, uint16 height);


#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_QEMUVGA_H_
