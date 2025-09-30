/**
 *  Amethyst Operating System - String functions.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_STRING_H_
#define INCLUDES_STRING_H_

#include <Types.h>

#ifdef __cplusplus
extern "C" {
#endif

    void string_copy(char *dest, const char* src);
    int string_compare(const char* a, const char* b);
    uint32 string_parseInt(const char* stringToParse);

    void string_toLower(char* string);
    void string_toUpper(char* string);

    uint32 string_length(const char* string);

    char** string_split(char* string, char splitter);

    /**
     * Formats a string.
     * @param destination Destination for the string output. Must have sufficient size. Memory allocation is the responsibility of the caller.
     * @param formatString TODO.
    */
//  void string_format(char* destination, const char* formatString, ...);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_STRING_H_
