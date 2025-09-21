#include <Graphics/canvas.h>
#include <Graphics/Widget.hpp>
#include <multiboot.h>
#include <Graphics/screenfont.h>

#ifndef MOUSEMOVEEVNT
#define MOUSEMOVEEVNT

class MouseMoveEvent {
  public:
    MouseMoveEvent(sint8 moveX, sint8 moveY);

    sint8 getMouseX();
    sint8 getMouseY();


  private:
    uint16 mouseX;
    uint16 mouseY;
};

#endif