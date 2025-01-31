#include <StandardIO.hpp>
#include <stream.h>

StandardIO::StandardIO(void (*stdout)(char), char (*stdin)(void)) {
    this->stdin = stdin;
    this->stdout = stdout;
}

void StandardIO::Print(const char* formatString, ...) {
    va_list args;
    va_start(args, formatString);

    stream_vprintf(this->stdout, formatString, args);

    va_end(args);
}

char* StandardIO::ReadLine(bool echo) {
    return stream_readLine(this->stdin, this->stdout, echo);
}
