/**
 *  Amethyst Operating System - Build-time kernel configuration.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include "Types.h"



#ifndef AMETHYST_H_
#define AMETHYST_H_

#ifdef	__cplusplus
extern "C" {
#endif


    #define VERSION_MAJOR 2024
    #define VERSION_MINOR 0
    #define VERSION_PATCH 0

    /**
     * The device to use in early-boot debug logging. Options are 'serial_writeChar' and 'vgaConsole_putChar'.
     */
    #define DEBUG_DEVICE serial_writeChar
    
    /**
     * Debug logging level. Any log level with a higher number than this won't be processed. 0 (critical) is highest, 4 (debug) is lowest.
     */
    #define DEBUG_LEVEL     4

#ifdef	__cplusplus
}
#endif

#endif // AMETHYST_H_s
