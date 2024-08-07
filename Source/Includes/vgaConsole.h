/**
 *  Amethyst Operating System - VGA text console functionality.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>
#include <stream.h>

#ifndef INCLUDES_VGACONSOLE_H_
#define INCLUDES_VGACONSOLE_H_

#ifdef __cplusplus
extern "C" {
#endif

    void vgaConsole_initialise(void);

    void vgaConsole_putChar(char c);


    Stream* vgaConsole_getStream(void);

    // VGA Text Palette, in easy to use form :P.
    #define VGACONSOLE_BLACK            0x0
    #define VGACONSOLE_BLUE             0x1
    #define VGACONSOLE_GREEN            0x2
    #define VGACONSOLE_CYAN             0x3
    #define VGACONSOLE_RED              0x4
    #define VGACONSOLE_MAGENTA          0x5
    #define VGACONSOLE_BROWN            0x6
    #define VGACONSOLE_LIGHT_GREY       0x7
    #define VGACONSOLE_DARK_GREY        0x8
    #define VGACONSOLE_LIGHT_BLUE       0x9
    #define VGACONSOLE_LIGHT_GREEN      0xA
    #define VGACONSOLE_LIGHT_CYAN       0xB
    #define VGACONSOLE_LIGHT_RED        0xC
    #define VGACONSOLE_LIGHT_MAGENTA    0xD
    #define VGACONSOLE_LIGHT_BROWN      0xE
    #define VGACONSOLE_WHITE            0xF

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_VGACONSOLE_H_
