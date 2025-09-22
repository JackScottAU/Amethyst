/**
 *  Amethyst Operating System - Desktop Widget.
 *  Copyright 2025 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Graphics/canvas.h>
#include <Graphics/Widget.hpp>
#include <multiboot.h>
#include <Graphics/screenfont.h>

#ifndef INCLUDES_GRAPHICS_DESKTOP_HPP_
#define INCLUDES_GRAPHICS_DESKTOP_HPP_

/**
 * The base widget in the GUI heirachy. Responsible for mouse pointer.
 */
class Desktop : public Widget {
  public:
    Desktop(Canvas* canvas);

    void Redraw();
    void HandleUIEvent(GuiEvent* eventData);

    using Widget::SetPosition;

  private:
    ScreenFont* font;
    const char* text;
    uint16 mouseX;
    uint16 mouseY;
    void DrawMouse();
};

#endif  // INCLUDES_GRAPHICS_DESKTOP_HPP_
