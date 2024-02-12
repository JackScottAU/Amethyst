/**
 *  Synergy Operating System - Generic FIFO Buffer.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_STRUCTURES_FIFOBUFFER_H_
#define INCLUDES_STRUCTURES_FIFOBUFFER_H_

#include <Types.h>

typedef struct {
    uint8* buffer;
    uint32 size;
    uint32 readIndex;
    uint32 writeIndex;
} FIFOBuffer;

/**
 * Creates a new FIFO buffer of the requested size.
*/
FIFOBuffer* FIFOBuffer_new(uint32 size);

/**
 * Writes the specified number of bytes from data into the buffer. Returns the number of bytes written.
*/
uint32 FIFOBuffer_WriteBytes(FIFOBuffer* buffer, uint8* data, uint32 size);

/**
 * Reads the specified number of bytes into data from the buffer. Returns the number of bytes read.
*/
uint32 FIFOBuffer_ReadBytes(FIFOBuffer* buffer, uint8* data, uint32 size);

uint32 FIFOBuffer_ContentsSize(FIFOBuffer* buffer);

#endif //INCLUDES_STRUCTURES_FIFOBUFFER_H_
