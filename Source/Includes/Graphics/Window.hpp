#include <Graphics/canvas.h>
#include <Graphics/Widget.hpp>
#include <multiboot.h>
#include <Graphics/screenfont.h>

/**
 * Like a program.
 */
class Window : public Widget {
  public:
    Window(ScreenFont* font, uint32 x, uint32 y, uint32 w, uint32 h, Canvas* canvas, char* name);

    void Redraw();
    void HandleUIEvent(void* eventData);

    void SetName(char* text);

    using Widget::SetPosition;

  private:
    ScreenFont* font;
    char* text;
};
