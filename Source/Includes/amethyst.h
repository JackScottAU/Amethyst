/**
 *  Amethyst Operating System - Build-time kernel configuration.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include "Types.h"
#include <debug.h>


#ifndef INCLUDES_AMETHYST_H_
#define INCLUDES_AMETHYST_H_

#ifdef __cplusplus
extern "C" {
#endif


#define VERSION_MAJOR 2025
#define VERSION_MINOR 2
#define VERSION_PATCH 4

/**
 * The device to use in early-boot debug logging. Options are 'serial_writeChar' and 'vgaConsole_putChar'.
 */
#define DEBUG_DEVICE serial_writeChar

/**
 * Debug logging level. Any log level with a higher number than this won't be processed. 0 (critical) is highest, 4 (debug) is lowest.
 */
#define DEBUG_LEVEL     LOGLEVEL_DEBUG

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_AMETHYST_H_
