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
    debug(LOGLEVEL_WARNING, "Performing Widget Redraw");
    children->Reset();
    debug(LOGLEVEL_WARNING, "after reset");

    do {
        Widget* child = children->Current();

        debug(LOGLEVEL_WARNING, "Child?");

        if(child != nullptr) {
            child->Redraw();
        }
        
    } while(children->Next());
}
