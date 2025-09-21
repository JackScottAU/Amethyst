#include <Graphics/Desktop.hpp>
#include <Graphics/TextLabel.hpp>
#include <Graphics/screenfont.h>
#include <string.h>
#include <debug.h>

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

void Desktop::HandleUIEvent(GuiEvent* eventData) {

    switch(eventData->GetEventType())
    {
        case GuiEventType::MOUSE_CLICK:
            debug(LOGLEVEL_DEBUG, "CLICK");
            // TODO: find out which child item has been clicked, and pass the click down to that child so it can process the event there.
            SendGuiEventToChildren(eventData, mouseX, mouseY);
            break;

        case GuiEventType::MOUSE_MOVE: {
            MouseMoveEvent* move = (MouseMoveEvent*) eventData;
            this->mouseX = this->mouseX + move->getMouseX();
            this->mouseY = this->mouseY + move->getMouseY();

            if(this->mouseX >= this->w) {
                this->mouseX = this->w - 1;
            }
            
            if(this->mouseY >= this->h) {
                this->mouseY = this->h - 1;
            }
            break;}

        default:
            debug(LOGLEVEL_ERROR, "UNKNOWN GUI EVENT!?");
            break;
    }

    Redraw();
}

void Desktop::DrawMouse() {
    canvas_drawRect(canvas, mouseX, mouseY, 1, 8, 0xFFFFFFFF);
    canvas_drawRect(canvas, mouseX, mouseY, 8, 1, 0xFFFFFFFF);
}
