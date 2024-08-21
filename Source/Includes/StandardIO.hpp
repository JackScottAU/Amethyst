/**
 *  Amethyst Operating System - Stream I/O definition.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_STANDARDIO_HPP_
#define INCLUDES_STANDARDIO_HPP_

#include <Types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

class StandardIO {
 public:
    StandardIO(void (*stdout)(char), char (*stdin)(void));

    void Print(const char* formatString, ...);

    char* ReadLine(bool echo);
    
 private:
    void (*stdout)(char);
    char (*stdin)(void);
};

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_STANDARDIO_HPP_
