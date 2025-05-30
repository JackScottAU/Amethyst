#include <Graphics/Window.hpp>
#include <Graphics/TextLabel.hpp>
#include <Graphics/screenfont.h>
#include <string.h>

Window::Window(ScreenFont* font, uint32 locx, uint32 locy, uint32 w, uint32 h, Canvas* canvas, const char* text)
{
    this->children = new LinkedList<Widget*>();
    this->canvas = canvas;
    this->x = locx;
    this->y = locy;
    this->text = text;
    this->font = font;

    this->h = h;
    this->w = w;

  //  TextLabel* label = new TextLabel(font, 8, 8, canvas, text, 0x00FFFFFF);

  //  children->Add(label);

    Redraw();
}

void Window::Redraw() {
    // Window Decoration
    canvas_drawRect(canvas, x, y, w, 32, 0x008000C0);

    uint32 textx = (w / 2) - (8 * string_length(text) / 2);

    screenfont_drawWord(canvas, font, x + textx, y + 8, 0xFFFFFFFF, text);

    RedrawChildren();
}

void Window::HandleUIEvent(void* eventData) {

}

void Window::SetName(char* text) {
    this->text = text;
    //this->w = font->header->width * string_length(text);

    Redraw();
}
