#include <Types.h>

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef STREAM_HPP_
#define STREAM_HPP_

class StreamC
{
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

#endif

#ifdef	__cplusplus
}
#endif