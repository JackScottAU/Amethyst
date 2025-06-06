/**
 *  Amethyst Operating System - Debug functions.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>
#include <Drivers/serial.h>
#include <amethyst.h>
#include <stdarg.h>
#include <stream.h>
#include <debug.h>

void debug(uint8 logLevel, const char* formatString, ...) {
    va_list args;
    va_start(args, formatString);

    if (logLevel > DEBUG_LEVEL) {
        return;
    }

    switch (logLevel) {
        case 0:
            stream_printf(DEBUG_DEVICE, "CRIT:   ");
            break;

        case 1:
            stream_printf(DEBUG_DEVICE, "ERROR:  ");
            break;

        case 2:
            stream_printf(DEBUG_DEVICE, "WARN:   ");
            break;

        case 3:
            stream_printf(DEBUG_DEVICE, "INFO:   ");
            break;

        case 4:
            stream_printf(DEBUG_DEVICE, "DEBUG:  ");
            break;
    }

    stream_vprintf(DEBUG_DEVICE, formatString, args);
    stream_printf(DEBUG_DEVICE, "\n");

    va_end(args);
}
