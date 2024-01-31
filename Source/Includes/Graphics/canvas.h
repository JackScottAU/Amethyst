/**
 *  Synergy Operating System - Graphical drawing canvas.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_GRAPHICS_CANVAS_H_
#define INCLUDES_GRAPHICS_CANVAS_H_

#include <Types.h>

typedef struct {
    void* framebuffer;
    uint32 width;
    uint32 height;
} Canvas;

#endif // INCLUDES_GRAPHICS_CANVAS_H_