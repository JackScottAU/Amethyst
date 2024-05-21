#ifndef INCLUDES_STREAM_H_
#define INCLUDES_STREAM_H_

#include <Types.h>
#include <Structures/fifobuffer.h>

#ifdef	__cplusplus
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

char* stream_readLine(bool echoMode);

/**
 * An input/output stream. If buffer is null, all writes are immediate.
*/
typedef struct {
    FIFOBuffer* buffer;
    void (*flush)(char);
} Stream;

#ifdef	__cplusplus
}
#endif

#endif // INCLUDES_STREAM_H_

