#include <Graphics/canvas.h>
#include <Graphics/screenfont.h>
#include <Graphics/Widget.hpp>

/***
 * Implements a scrolling ANSI compliant terminal within a framebuffer canvas.
 */
class TextConsole : public Widget {
  public:
    TextConsole(Canvas* canvas, ScreenFont* font, uint32 x, uint32 y, uint16 rows, uint16 columns);
    void HandleUIEvent(void* eventData);

    void PutChar(char c);

    void Redraw();

    void Scroll();

  private:
  void HandleControlSequenceIntroducer(char c);
  void HandleSelectGraphicsRendition();
  void DecodeSGR(uint32 parameter) ;
    char** characterBuffer;
    uint32** backColourBuffer;
    uint32** foreColourBuffer;
    uint16 rows;
    uint16 columns;
    uint16 currentRow;
    uint16 currentColumn;
    ScreenFont* font;
    Canvas* canvas;
    uint32 colour;
    uint32 backcolour;
    uint32 x;
    uint32 y;
    char* csiParameters;
    bool isInCSI;
};
