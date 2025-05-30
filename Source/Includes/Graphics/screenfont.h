/**
 *  Amethyst Operating System - PC Screen Font.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_GRAPHICS_SCREENFONT_H_
#define INCLUDES_GRAPHICS_SCREENFONT_H_

#include <Types.h>
#include <Graphics/canvas.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32 magicBytes;
    uint32 version;
    uint32 headerSize;
    uint32 flags;
    uint32 length;      // number of glyphs
    uint32 glyphSize;   // bytes per glyph
    uint32 height;      // pixels
    uint32 width;       // pixels
} ScreenFontHeader;

typedef struct {
    ScreenFontHeader* header;
    uint8* characterData;
} ScreenFont;


void screenfont_drawChar(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, char a);
void screenfont_drawWord(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, const char* a);

#define SCREENFONT_MAGIC 0x72B54A86

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_GRAPHICS_SCREENFONT_H_
