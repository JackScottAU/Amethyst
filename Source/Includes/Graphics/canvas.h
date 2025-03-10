/**
 *  Amethyst Operating System - Graphical drawing canvas.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_GRAPHICS_CANVAS_H_
#define INCLUDES_GRAPHICS_CANVAS_H_

#include <Types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* framebuffer;
    uint32 width;
    uint32 height;
    // TODO: if we include bytes per line in here, we can make sub-canvas that can be given to widget children.
} Canvas;

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_GRAPHICS_CANVAS_H_
