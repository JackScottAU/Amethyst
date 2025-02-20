/**
 *  Amethyst Operating System - Kernel debugging functionality.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include "Types.h"



#ifndef INCLUDES_DEBUG_H_
#define INCLUDES_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * Writes kernel debugging information to the kernel's debug log (serial port in early boot, log files once booted).
     * @param logLevel The level of the log message.
     * @param formatString printf()-formatted message.
     */
    void debug(uint8 logLevel, const char* formatString, ...);

    #define LOGLEVEL_CRITICAL   0
    #define LOGLEVEL_ERROR      1
    #define LOGLEVEL_WARNING    2
    #define LOGLEVEL_INFO       3
    #define LOGLEVEL_DEBUG      4

    /**
     * Recommended level for high-volume messages such as internal memory manager and scheduler decisions.
     */
    #define LOGLEVEL_TRACE      5

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_DEBUG_H_
