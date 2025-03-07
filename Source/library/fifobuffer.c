/**
 *  Amethyst Operating System - Generic FIFO Buffer.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>
#include <Structures/fifobuffer.h>
#include <memoryManager.h>

FIFOBuffer* FIFOBuffer_new(uint32 size) {
    FIFOBuffer* buffer = memoryManager_allocate(sizeof(FIFOBuffer));

    buffer->buffer = memoryManager_allocate(size);
    buffer->size = size;
    buffer->readIndex = 0;
    buffer->writeIndex = 0;

    return buffer;
}

uint32 FIFOBuffer_WriteBytes(FIFOBuffer* buffer, uint8* data, uint32 size) {
    uint32 i;
    const uint8 * p;
    p = data;
    for (i = 0; i < size; i++) {
        // first check to see if there is space in the buffer
        if ( (buffer->writeIndex + 1 == buffer->readIndex) ||
            ( (buffer->writeIndex + 1 == buffer->size) && (buffer->readIndex == 0) )) {
                return i;  // no more room
        } else {
            buffer->buffer[buffer->writeIndex] = *p++;
            buffer->writeIndex++;  // increment the head

            if ( buffer->writeIndex == buffer->size ) {  // check for wrap-around
                buffer->writeIndex = 0;
            }
        }
    }

    return size;
}

uint32 FIFOBuffer_ReadBytes(FIFOBuffer* buffer, uint8* data, uint32 size) {
    uint32 i;
    uint8 * p;
    p = data;
    for (i = 0; i < size; i++) {
        if ( buffer->readIndex != buffer->writeIndex ) {    // see if any data is available
            *p++ = buffer->buffer[buffer->readIndex];  // grab a byte from the buffer
            buffer->readIndex++;  // increment the tail
            if (buffer->readIndex == buffer->size) {  // check for wrap-around
                buffer->readIndex = 0;
            }
        } else {
            return i;   // number of bytes read
        }
    }

    return size;
}

uint32 FIFOBuffer_ContentsSize(FIFOBuffer* buffer) {
    if (buffer->readIndex == buffer->writeIndex) {
        return 0;
    }

    if (buffer->readIndex > buffer->writeIndex) {
        return buffer->readIndex - buffer->writeIndex;
    }

    // TODO(JackScottAU): Pretty sure there's a bug here. Will (might) fix later.
    return buffer->writeIndex - buffer->readIndex;
}
