/**
 *  Amethyst Operating System - VESA Framebuffer.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Graphics/canvas.h>
#include <Graphics/screenfont.h>

void canvas_putPixel(Canvas* canvas, uint32 x, uint32 y, uint32 colour) {
    uint32* screen = canvas->framebuffer;
    screen[(y * canvas->width) + x] = colour;
}

void canvas_drawRect(Canvas* canvas, uint16 x, uint16 y, uint16 w, uint16 h, uint32 colour) {
    for (uint16 i = 0; i < h; i++) {
        for (uint16 j = 0; j < w; j++) {
            canvas_putPixel(canvas, x + j, y + i, colour);
        }
    }
}

void screenfont_drawChar(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, char a) {
    // go to top level.
    uint8* charData = font->characterData;
    charData += a * font->header->height;

    for (uint32 r = 0; r < font->header->height; r++) {
        // draw each row.

        for (uint32 c = 0; c < font->header->width; c++) {
            // draw each column/pixel.
            char yes = (charData[r] << c) & 0x80;

            if (yes) {
                canvas_putPixel(canvas, x + c, y + r, colour);
            }
        }
    }
}

void screenfont_drawWord(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, const char* a) {
    int i = 0;

    while (a[i]) {
        screenfont_drawChar(canvas, font, x + (i*font->header->width), y, colour, a[i]);
        i++;
    }
}
