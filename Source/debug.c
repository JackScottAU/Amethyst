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

void egaPutChar(char out);

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
    stream_vprintf(egaPutChar, formatString, args);
    stream_printf(DEBUG_DEVICE, "\n");
    stream_printf(egaPutChar, "\n");

    va_end(args);
}

char* egaFB = (char*) 0xC00B8000;

uint32 egaPos = 0;

void egaPutChar(char out) {
    if (out == '\n') {
        egaPos = 0;
    } else {
        egaFB[egaPos] = out;
        egaPos++;
        egaFB[egaPos] = 0x0F;
        egaPos++;
    }
}
