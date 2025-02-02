#include <Graphics/canvas.h>
#include <Graphics/screenfont.h>
#include "../../drivers/vesa_framebuffer.h"

class TextBox {
  public:
    TextBox(Canvas* canvas, ScreenFont* font, uint16 rows, uint16 columns);

    void PutChar(char c);

    void Redraw();

    void Scroll();

  private:
    char** characterBuffer;
    uint16 rows;
    uint16 columns;
    uint16 currentRow;
    uint16 currentColumn;
    ScreenFont* font;
    Canvas* canvas;
    uint32 colour;
};
