/**
 *  Amethyst Operating System - Standard types
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_TYPES_H_
#define INCLUDES_TYPES_H_

#include <stdbool.h>

    // Basic Type Definitions.
    typedef signed      char        sint8;
    typedef unsigned    char        uint8;
    typedef signed      short       sint16;
    typedef unsigned    short       uint16;
    typedef signed      long        sint32;
    typedef unsigned    long        uint32;
    typedef signed      long long   sint64;
    typedef unsigned    long long   uint64;

    #define NULL ((void*) 0x00000000)

#endif  // INCLUDES_TYPES_H_
