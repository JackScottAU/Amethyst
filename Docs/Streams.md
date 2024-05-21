# Streams

One of the fundamental constructs within the operating system will be streams (and one we'll need quite early on for text input and output to the user). A stream is an object that can either be a source of byte data, or a destination for byte data. Bytes are written one at a time or read one at a time.

Streams cannot be seeked (otherwise they're just files).

Streams can be read (keyboard) or write (vga), or both (serial).

Streams are ANSI terminal escape compliant (so keyboard sequences can be encoded for passing up to applications).

    stream_getMode(); // stream can be read from, written to, or both.

    stream_canRead(); // buffer has data.
    stream_readChar();
    stream_readString(); // reads until buffer is empty.
    stream_readLine(); // reads until an enter is hit.

    stream_canWrite(); // buffer not full.
    stream_writeChar();
    stream_writeString(); // writes until a /0.
    stream_writeLine(); // writeString but with a /n at the end.

streams would be easier with C++ classes?

STREAM_CANREAD = 0
STREAM_CANWRITE =1

ANSI streams: carriage return is idempotent, new line/line feed is not.

StreamReader has support functions more than ReadByte()/ReadBytes()
StreamWriter ditto


Classes:
    Stream
    StreamReader
    StreamWriter

    MemoryStream (FIFO Buffer)
    SerialStream
        new SerialStream(serialDevice);
    ConsoleStream
        new ConsoleStream(vgaDevice, keyboardDevice);