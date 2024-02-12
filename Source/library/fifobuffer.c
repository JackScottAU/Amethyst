/**
 *  Synergy Operating System - Generic FIFO Buffer.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>
#include <Structures/fifobuffer.h>
#include <memoryManager.h>

FIFOBuffer* FIFOBuffer_new(uint32 size) {
    FIFOBuffer* buffer = memoryManager_allocate(sizeof(FIFOBuffer));

    buffer->buffer = memoryManager_allocate(size);
    buffer->bufferSize = size;
    buffer->bufferReadIndex = 0;
    buffer->bufferWriteIndex = 0;

    return buffer;
}

uint32 FIFOBuffer_WriteBytes(FIFOBuffer* buffer, uint8* data, uint32 size) {
    uint32 i;
    const char * p;
    p = data;
    for(i=0; i < size; i++){
        //first check to see if there is space in the buffer
        if( (buffer->bufferWriteIndex + 1 == buffer->bufferReadIndex) ||
            ( (buffer->bufferWriteIndex + 1 == buffer->bufferSize) && (buffer->bufferReadIndex == 0) )){
                return i; //no more room
        } else {
            buffer->buffer[buffer->bufferWriteIndex] = *p++;
            buffer->bufferWriteIndex++;  //increment the head
            if( buffer->bufferWriteIndex == buffer->bufferSize ){  //check for wrap-around
                buffer->bufferWriteIndex = 0;
            }
        }
    }

    return size;
}

uint32 FIFOBuffer_ReadBytes(FIFOBuffer* buffer, uint8* data, uint32 size) {
    uint32 i;
    uint8 * p;
    p = data;
    for(i=0; i < size; i++){
        if( buffer->bufferReadIndex != buffer->bufferWriteIndex ){ //see if any data is available
            *p++ = buffer->buffer[buffer->bufferReadIndex];  //grab a byte from the buffer
            buffer->bufferReadIndex++;  //increment the tail
            if( buffer->bufferReadIndex == buffer->bufferSize ){  //check for wrap-around
                buffer->bufferReadIndex = 0;
            }
        } else {
            return i; //number of bytes read
        }
    }

    return size;
}

uint32 FIFOBuffer_ContentsSize(FIFOBuffer* buffer) {
    if(buffer->bufferReadIndex == buffer->bufferWriteIndex) {
        return 0;
    }

    if(buffer->bufferReadIndex > buffer->bufferWriteIndex) {
        return buffer->bufferReadIndex - buffer->bufferWriteIndex;
    }

    // Pretty sure there's a bug here. Will (might) fix later.
    return buffer->bufferWriteIndex - buffer->bufferReadIndex;
}
