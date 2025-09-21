#include <Graphics/canvas.h>
#include <Graphics/Widget.hpp>
#include <multiboot.h>
#include <Graphics/screenfont.h>

/**
 * Like a window manager.
 */
class Desktop : public Widget {
  public:
    Desktop(Canvas* canvas);

    void Redraw();
    void HandleUIEvent(MouseMoveEvent* eventData);

    using Widget::SetPosition;

  private:
    ScreenFont* font;
    const char* text;
    uint16 mouseX;
    uint16 mouseY;
    void DrawMouse();
};
