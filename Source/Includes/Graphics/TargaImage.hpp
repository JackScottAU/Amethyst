#include <Graphics/canvas.h>
#include <Graphics/Widget.hpp>

/**
 * A very simple image widget that loads data from a .tga image file and draws it on the canvas.
 */
class TargaImage : public Widget {
  public:
    TargaImage(uint8* ptr, uint32 length, uint32 x, uint32 y, Canvas* canvas);

    void Redraw();
    void HandleUIEvent(GuiEvent* eventData);

    using Widget::SetPosition;

  private:
    uint32* pixels;
};
