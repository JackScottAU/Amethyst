/**
 *  Amethyst Operating System - Base UI Element.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_GRAPHICS_WIDGET_H_
#define INCLUDES_GRAPHICS_WIDGET_H_

#include <Types.h>
#include <Graphics/canvas.h>
#include <Structures/list.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Widget_s {
    Canvas* canvas;
    uint32 x;
    uint32 y;
    uint32 width;
    uint32 height;

    struct Widget_s* parent;
    List* children;
} Widget;

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_GRAPHICS_WIDGET_H_
