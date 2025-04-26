/**
 *  Amethyst Operating System - VESA Framebuffer.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Graphics/canvas.h>
#include <Graphics/screenfont.h>
#include <Types.h>

#ifndef DRIVERS_VESA_FRAMEBUFFER_H_
#define DRIVERS_VESA_FRAMEBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

void vga_putPixel(Canvas* canvas, uint32 x, uint32 y, uint32 colour);
void vga_drawRect(Canvas* canvas, uint16 x, uint16 y, uint16 w, uint16 h, uint32 colour);
void vga_drawChar(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, char a);
void vga_drawWord(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, const char* a);

/**
 * Copies width*height pixels from startx,starty to endx,endy.
 */
//void canvas_copy(Canvas* canvas, uint32 startx, uint32 starty, uint32 endx, uint32 endy, uint32 width, uint32 height);

#ifdef __cplusplus
}
#endif


#endif  // DRIVERS_VESA_FRAMEBUFFER_H_
