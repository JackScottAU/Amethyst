Streams cannot be seeked (otherwise they're just files).

Streams can be read (keyboard) or write (vga), or both (serial).

Streams are ANSI terminal escape compliant.

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