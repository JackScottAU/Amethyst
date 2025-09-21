#include <Graphics/canvas.h>
#include <Graphics/Widget.hpp>
#include <multiboot.h>
#include <Graphics/screenfont.h>

/**
 * Uses a screenfont to display text.
 */
class TextLabel : public Widget {
  public:
    TextLabel(ScreenFont* font, uint32 x, uint32 y, Canvas* canvas, char* text, uint32 colour);

    void Redraw();
    void HandleUIEvent(GuiEvent* eventData);

    void SetFont(ScreenFont* font);
    void SetText(char* text);

    using Widget::SetPosition;

  private:
    ScreenFont* font;
    char* text;
    uint32 colour;
};
