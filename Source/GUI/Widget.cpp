#include <Graphics/Widget.hpp>
#include <memoryManager.h>
#include <Types.h>
#include <debug.h>
#include <Graphics/canvas.h>

void Widget::SetPosition(sint32 x, sint32 y) {
    this->x = x;
    this->y = y;
}

void Widget::AddChild(Widget* widget) {
    children->Add(widget);
}

void Widget::RemoveChild(Widget* widget) {
    children->Remove(widget);
}

void Widget::RedrawChildren()
{
    debug(LOGLEVEL_TRACE, "Performing Widget Redraw");
    children->Reset();
    debug(LOGLEVEL_TRACE, "after reset");

    do {
        Widget* child = children->Current();

        debug(LOGLEVEL_TRACE, "Child?");

        if(child != nullptr) {
            child->Redraw();
        }
        
    } while(children->Next());
}

void Widget::SendGuiEventToChildren(GuiEvent* eventData, uint32 x, uint32 y) {
    debug(LOGLEVEL_TRACE, "Performing Event Pushdown");
    children->Reset();

    do {
        Widget* child = children->Current();

        debug(LOGLEVEL_TRACE, "Child?");

        if(child != nullptr) {
            debug(LOGLEVEL_TRACE, "Event X: %h, Child X: %h, Child W: %h", x, child->x, child->w);
            debug(LOGLEVEL_TRACE, "Event Y: %h, Child Y: %h, Child H: %h", y, child->y, child->h);


            if(x >= child->x && x <= child->x + child->w) {
                debug(LOGLEVEL_TRACE, "X");
                if(y >= child->y && y <= child->y + child->h) {
                    debug(LOGLEVEL_TRACE, "Y");
                    child->HandleUIEvent(eventData);
                }
            }
        }
        
    } while(children->Next());
}