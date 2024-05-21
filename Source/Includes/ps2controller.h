/**
 *  Synergy Operating System - Driver for 8042 PS/2 Keyboard and Mouse Controller.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_PS2CONTROLLER_H_
#define INCLUDES_PS2CONTROLLER_H_

#include <deviceTree.h>

#ifdef	__cplusplus
extern "C" {
#endif

deviceTree_Entry* ps2controller_initialise(void);

#ifdef	__cplusplus
}
#endif

#endif // INCLUDES_PS2CONTROLLER_H_