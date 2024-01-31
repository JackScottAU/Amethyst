/**
 *  Synergy Operating System - PC Screen Font.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_GRAPHICS_SCREENFONT_H_
#define INCLUDES_GRAPHICS_SCREENFONT_H_

#include <Types.h>

typedef struct {
    uint8 characterSize;
    uint8 width;
    uint8 height;
    uint8* characterData;
} ScreenFont;

#endif // INCLUDES_GRAPHICS_SCREENFONT_H_