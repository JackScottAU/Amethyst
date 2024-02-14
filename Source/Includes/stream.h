#ifndef INCLUDES_STREAM_H_
#define INCLUDES_STREAM_H_

#include <Types.h>

/**
 * Prints a formatted string to the specified putChar function (the putChar should take a single argument which is a char).
 * %d - decimal
 * %h - hex, no padding
 * %H - hex, padding
 * %s - string
*/
void stream_printf(void (*putChar)(char), const char* formatString, ...);

char* stream_readLine(bool echoMode);

#endif // INCLUDES_STREAM_H_

