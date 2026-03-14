/**
 *  Amethyst Operating System - Mouse move event functions.
 *  Copyright 2026 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Graphics/MouseMoveEvent.hpp>

MouseMoveEvent::MouseMoveEvent(sint8 moveX, sint8 moveY) {
    this->mouseX = moveX;
    this->mouseY = moveY;
}

sint8 MouseMoveEvent::getMouseX() {
    return this->mouseX;
}

sint8 MouseMoveEvent::getMouseY() {
    return this->mouseY;
}

GuiEventType MouseMoveEvent::GetEventType() {
    return GuiEventType::MOUSE_MOVE;
}

GuiEventType MouseClickEvent::GetEventType() {
    return GuiEventType::MOUSE_CLICK;
}
