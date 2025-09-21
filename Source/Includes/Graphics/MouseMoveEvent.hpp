#include <Graphics/canvas.h>
#include <Graphics/Widget.hpp>
#include <multiboot.h>
#include <Graphics/screenfont.h>

#ifndef MOUSEMOVEEVNT
#define MOUSEMOVEEVNT

enum GuiEventType {
  MOUSE_CLICK,

  MOUSE_MOVE,
};

class GuiEvent {
  public:
    virtual GuiEventType GetEventType() = 0;
};

class MouseClickEvent : public GuiEvent {
  public:
    GuiEventType GetEventType();
};

class MouseMoveEvent : public GuiEvent {
  public:
    MouseMoveEvent(sint8 moveX, sint8 moveY);

    sint8 getMouseX();
    sint8 getMouseY();

    GuiEventType GetEventType();


  private:
    uint16 mouseX;
    uint16 mouseY;
};

#endif