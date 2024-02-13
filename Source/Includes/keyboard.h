#ifndef INCLUDES_KEYBOARD_H_
#define INCLUDES_KEYBOARD_H_

#include <deviceTree.h>
#include <Types.h>

deviceTree_Entry* keyboard_initialise(void);
char keyboard_readChar(void);

#endif // INCLUDES_KEYBOARD_H_
