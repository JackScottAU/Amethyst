#include <Graphics/TextLabel.hpp>
#include <Graphics/screenfont.h>
#include <string.h>

TextLabel::TextLabel(ScreenFont* font, uint32 locx, uint32 locy, Canvas* canvas, char* text, uint32 colour)
{
    this->children = new LinkedList<Widget*>();
    this->canvas = canvas;
    this->x = locx;
    this->y = locy;
    this->text = text;
    this->font = font;
    this->colour = colour;

    this->h = font->header->height;
    this->w = font->header->width * string_length(text);

    Redraw();
}

void TextLabel::Redraw() {
    // TODO
    screenfont_drawWord(canvas, font, x, y, colour, text);
}

void TextLabel::HandleUIEvent(GuiEvent* eventData) {

}

void TextLabel::SetFont(ScreenFont* font) {
    this->font = font;
    this->h = font->header->height;
    this->w = font->header->width * string_length(text);

    Redraw();
}

void TextLabel::SetText(char* text) {
    this->text = text;
    this->w = font->header->width * string_length(text);

    Redraw();
}
