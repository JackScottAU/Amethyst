/**
 *  Amethyst Operating System - String manipulation functions.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <string.h>
#include <serial.h>
#include <stream.h>
#include <memoryManager.h>
#include <debug.h>

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

void string_toLower(char* string) {
    while (*string) {
        if (*string >= 0x41 && *string <= 0x5A) {
            *string += 0x20;
        }
        string++;
    }
}

char** string_split(char* string, char splitter) {
    char** strings = memoryManager_allocate(sizeof(char*) * 128); // to store pointers. TODO: count splitter occurences beforehand so we allocate the right amount.

    int i = 0;
    char* stringStart = string;

    while(*string != NULL) {
      //  debug(LOGLEVEL_DEBUG, "Remaining text: %s", string);

        if(*string == splitter || *string == '\0') {
        //    debug(LOGLEVEL_DEBUG, "Match");
            strings[i] = stringStart;
            *string = '\0';
            stringStart = string + 1;
            i++;

        }

        string++;
    }
    strings[i] = stringStart;
    strings[i + 1] = NULL;

    return strings;
}

uint32 string_length(char* string) {
    uint32 len = 0;

    while (*string) {
        len++;
        string++;
    }

    return len;
}

void string_toUpper(char* string) {
    while (*string) {
        if (*string >= 0x61 && *string <= 0x6A) {
            *string -= 0x20;
        }
        string++;
    }
}

uint32 string_parseInt(const char* string) {
    int result = 0;

    for (int i = 0; string[i] <= '9' && string[i] >= '0'; ++i) {
        result = result * 10 + string[i] - '0';
    }

    return result;
}
