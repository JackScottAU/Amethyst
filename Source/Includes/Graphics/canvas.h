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

void canvas_putPixel(Canvas* canvas, uint32 x, uint32 y, uint32 colour);
void canvas_drawRect(Canvas* canvas, uint16 x, uint16 y, uint16 w, uint16 h, uint32 colour);

/**
 * Copies width*height pixels from startx,starty to endx,endy.
 */
// void canvas_copy(Canvas* canvas, uint32 startx, uint32 starty, uint32 endx, uint32 endy, uint32 width, uint32 height);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_GRAPHICS_CANVAS_H_
