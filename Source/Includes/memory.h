/**
 *  Amethyst Operating System - Memory
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_MEMORY_H_
#define INCLUDES_MEMORY_H_

#ifdef __cplusplus
extern "C" {
#endif


void* memset(void *b, int c, int len);
void memzero(void* b, int len) ;

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_MEMORY_H_
