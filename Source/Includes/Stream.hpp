/**
 *  Amethyst Operating System - Stream I/O definition.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/
#include <Types.h>

#ifndef INCLUDES_STREAM_HPP_
#define INCLUDES_STREAM_HPP_

#ifdef __cplusplus
extern "C" {
#endif

class StreamC {
 public:
    virtual bool CanSeek();
    virtual void Seek(uint32 where);

    virtual bool CanWrite();
    virtual void WriteByte(uint8 byte);

    // Returns number of bytes written.
    virtual uint32 WriteBytes(uint8* data, uint32 size);

    virtual bool CanRead();
    virtual uint8 ReadByte();

    // Reads up to size bytes (if available) and returns number of bytes read.
    virtual uint32 ReadBytes(uint8* data, uint32 size);
};

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_STREAM_HPP_
