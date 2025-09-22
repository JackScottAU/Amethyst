/**
 *  Amethyst Operating System - Base UI Element.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_GRAPHICS_WIDGET_HPP_
#define INCLUDES_GRAPHICS_WIDGET_HPP_

#include <Types.h>
#include <Graphics/canvas.h>
#include <Structures/linkedlist.hpp>
#include <Graphics/MouseMoveEvent.hpp>

/**
 * Abstract class for a graphical widget. Has an X and Y, width and height, knows how to draw itself and accept UI events, and has children it can draw and pass events to.
 */
class Widget {
 public:
  virtual void Redraw() = 0;
  virtual void HandleUIEvent(GuiEvent* eventData) = 0;

  void SetPosition(sint32 x, sint32 y);

  // Children functions.
  void AddChild(Widget* widget);
  void RemoveChild(Widget* widget);

 protected:
  Canvas* canvas;
  Widget* parent;
  LinkedList<Widget*>* children;
  sint32 x;
  sint32 y;
  uint32 h;
  uint32 w;

  void RedrawChildren();
  void SendGuiEventToChildren(GuiEvent* eventData, uint32 x, uint32 y);
};

#endif  // INCLUDES_GRAPHICS_WIDGET_HPP_
