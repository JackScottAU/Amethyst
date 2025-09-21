#include <Graphics/Desktop.hpp>
#include <Graphics/TextLabel.hpp>
#include <Graphics/screenfont.h>
#include <string.h>

Desktop::Desktop(Canvas* canvas)
{
    this->children = new LinkedList<Widget*>();
    this->canvas = canvas;
    this->text = text;
    this->font = font;

    this->x = 0;
    this->y = 0;
    this->h = canvas->height;
    this->w = canvas->width;

    this->mouseX = canvas->width / 2;
    this->mouseY = canvas->height / 2;

    Redraw();
}

void Desktop::Redraw() {
    RedrawChildren();
    DrawMouse();
}

void Desktop::HandleUIEvent(MouseMoveEvent* eventData) {
    this->mouseX = this->mouseX + eventData->getMouseX();
    this->mouseY = this->mouseY + eventData->getMouseY();

    if(this->mouseX >= this->w) {
        this->mouseX = this->w - 1;
    }
    
    if(this->mouseY >= this->h) {
        this->mouseY = this->h - 1;
    }

    Redraw();
}

void Desktop::DrawMouse() {
    canvas_drawRect(canvas, mouseX, mouseY, 1, 8, 0xFFFFFFFF);
    canvas_drawRect(canvas, mouseX, mouseY, 8, 1, 0xFFFFFFFF);
}
