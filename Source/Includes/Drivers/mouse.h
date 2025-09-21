/**
 *  Amethyst Operating System - PS/2 Keyboard driver.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_MOUSE_H_
#define INCLUDES_MOUSE_H_

#include <deviceTree.h>
#include <Types.h>


#ifdef __cplusplus
extern "C" {
#endif

deviceTree_Entry* mouse_initialise(void);

// When we convert mouse to be a C++ file we can do something like mouse_registerRootWidget() instead.
void sortOfMouse_HandleEvent(sint16 moveX, sint16 moveY);
void sortOfMouse_HandleClickEvent();

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_MOUSE_H_
