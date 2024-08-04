/**
 *  Amethyst Operating System - String manipulation functions.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <string.h>
#include <serial.h>
#include <stream.h>

void string_copy(char *dest, const char* src) {
    while (*src) {      // while source string isnt null, copy, then increase pointer.
        *dest = *src;
        dest++;
        src++;
    }
}

int string_compare(const char* a, const char* b) {
    while (*a && *a == *b) {
         ++a; ++b;
    }

    return (int)(unsigned char)(*a) - (int)(unsigned char)(*b);
}

uint32 string_parseInt(const char* string) {
    int result = 0;

    for (int i = 0; string[i] <= '9' && string[i] >= '0'; ++i) {
        result = result * 10 + string[i] - '0';
    }

    return result;
}
