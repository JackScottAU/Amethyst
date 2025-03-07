/**
 *  Amethyst Operating System - Memory manipulation functions.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <memory.h>

void* memset(void *b, int c, int len) {
    unsigned char *p = (unsigned char*)b;

    while (len > 0) {
        *p = c;
        p++;
        len--;
    }
    return(b);
}

void memzero(void* b, int len) {
    memset(b, 0, len);
}
