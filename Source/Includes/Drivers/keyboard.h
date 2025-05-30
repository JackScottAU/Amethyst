/**
 *  Amethyst Operating System - PS/2 Keyboard driver.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_KEYBOARD_H_
#define INCLUDES_KEYBOARD_H_

#include <deviceTree.h>
#include <Types.h>


#ifdef __cplusplus
extern "C" {
#endif

deviceTree_Entry* keyboard_initialise(void);
char keyboard_readChar(void);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_KEYBOARD_H_
