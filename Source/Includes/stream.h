/**
 *  Amethyst Operating System - Stream definition.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_STREAM_H_
#define INCLUDES_STREAM_H_

#include <Types.h>
#include <Structures/fifobuffer.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Prints a formatted string to the specified putChar function (the putChar should take a single argument which is a char).
 * %d - decimal
 * %h - hex, no padding
 * %H - hex, padding
 * %s - string
*/
void stream_printf(void (*putChar)(char), const char* formatString, ...);

// Internal version of the above, mostly for use with debug etc.
void stream_vprintf(void (*putChar)(char), const char* formatString, va_list args);

char* stream_readLine(char (*getChar)(void), void (*putChar)(char), bool echoMode);

/**
 * An input/output stream. If buffer is null, all writes are immediate.
*/
typedef struct {
    FIFOBuffer* buffer;
    void (*flush)(char);
} Stream;

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_STREAM_H_
